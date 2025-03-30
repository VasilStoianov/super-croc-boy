#pragma once
#include "player.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

const float GRAVITY = 9.81;
int debug = 0;
float dt = 1.f / 60.f;

typedef enum CollisionDirection { LEFT, RIGHT, TOP, BOTTOM } CollisionDirection;

float min(float a, float b) {
  if (a > b) {
    return b;
  } else {
    return a;
  }
}

void updatePlayer(Player *player) {

  if (player->up) {
    if (!player->inAir) {

      playerJump(player);
    }
  } else if (player->left) {
    player->velocity.x = (-player->playerAcceleration);

    player->index = hash("run") % animations;
    player->animFrames = player->frames[player->index];
    player->scale.x = -.125f;
  } else if (player->right) {
    player->velocity.x = (player->playerAcceleration);
    player->scale.x = .125f;
    player->index = hash("run") % animations;
    player->animFrames = player->frames[player->index];

  } else if (player->down) {

  } else {
    player->velocity.x = 0;
    player->index = hash("idle") % animations;
    player->frame = 0;
    player->animFrames = player->frames[player->index];
  }

  if (player->velocity.y < -player->maxYvelocity) {
    player->velocity.y +=player->maxYvelocity*dt;
    player->mass = 35;
   printf("Max velocity\n");
  }
  player->position.x += player->velocity.x;
  player->position.y += player->velocity.y;
}

void applyGravity(Player **p) {
  (*p)->velocity.y += (GRAVITY / (*p)->mass) ;
  (*p)->position.y += ((*p)->velocity.y)*dt;
  // (*p)->mass++;
  // (*p)->velocity.y += (*p)->mass * dt;
  // (*p)->position.y += (*p)->velocity.y * dt;
}

sfBool aabb(sfSprite *sprite, sfRectangleShape *rect) {
  sfFloatRect playerOrigin = sfSprite_getGlobalBounds(sprite);
  sfFloatRect rectOrigin = sfRectangleShape_getGlobalBounds(rect);

  return playerOrigin.top + playerOrigin.height > rectOrigin.top &&
         playerOrigin.left + playerOrigin.width > rectOrigin.left &&
         playerOrigin.left < rectOrigin.left + rectOrigin.width &&
         playerOrigin.top < rectOrigin.top + rectOrigin.height;
}

void handleCollision(Player *player, sfSprite *sprite, sfRectangleShape *rect) {

  sfBool ox = aabb(sprite, rect);
  if (ox) {

    sfFloatRect playerOrigin = sfSprite_getGlobalBounds(sprite);
    sfFloatRect rectOrigin = sfRectangleShape_getGlobalBounds(rect);

    float overlapXA =
        (playerOrigin.left + playerOrigin.width) - rectOrigin.left;
    float overlapXB = (rectOrigin.left + rectOrigin.width) - playerOrigin.left;
    float overlapYA = (playerOrigin.top + playerOrigin.height) - rectOrigin.top;
    float overlapYB = (rectOrigin.top + rectOrigin.width) - playerOrigin.top;
    float mixOverlapX = min(overlapXA, overlapXB);
    float mixOverlapY = min(overlapYA, overlapYB);
    CollisionDirection direction;
    if (mixOverlapX < mixOverlapY) {
      if (overlapXA < overlapXB) {
        direction = LEFT;
      } else {
        direction = RIGHT;
      }
    } else {
      if (overlapYA < overlapYB) {
        direction = TOP;
      } else {
        direction = BOTTOM;
      }
    }

    switch (direction) {
    case TOP: {
      player->velocity.y = 0;
      player->position.y = rectOrigin.top - playerOrigin.height / 2;
      // player->mass = 0;
      player->inAir = 0;
      break;
    };
    case LEFT: {
      player->position.x = rectOrigin.left - playerOrigin.height / 3;
      break;
    };
    case BOTTOM: {
      player->position.y = (rectOrigin.top +  rectOrigin.height) + playerOrigin.height/3;
      // player->velocity.y += -55;
    }
    case RIGHT: {

      player->position.x = rectOrigin.left + playerOrigin.height / 3;
    }
    default:
      break;
    }
    if (player->velocity.x == 0) {
      player->index = hash("idle") % animations;
      player->frame = 0;
      player->animFrames = player->frames[player->index];
    }
  }
}

void handleKeyInput(sfEvent event, sfRenderWindow *renderer, Player *player) {
  if (event.key.type == sfEvtClosed) {
    sfRenderWindow_close(renderer);
  }
  if (event.key.code == sfKeyEscape) {
    sfRenderWindow_close(renderer);
  }
  if (event.key.code == sfKeyD && event.type == sfEvtKeyPressed) {
    debug = !debug;
  }
  // if (event.key.code == sfKeyRight && event.type == sfEvtKeyPressed) {
  if (sfKeyboard_isKeyPressed(sfKeyRight)) {
    player->right = 1;
  }
  // if (event.key.code == sfKeyLeft && event.type == sfEvtKeyPressed) {
  if (sfKeyboard_isKeyPressed(sfKeyLeft)) {
    player->left = 1;
  }
  if (event.key.code == sfKeyRight && event.type == sfEvtKeyReleased) {
    player->right = 0;
  }
  if (event.key.code == sfKeyLeft && event.type == sfEvtKeyReleased) {
    player->left = 0;
  }
  if (sfKeyboard_isKeyPressed(sfKeyLShift)) {
    player->velocity.x = player->velocity.x * 2;
  }
  if (sfKeyboard_isKeyPressed(sfKeyUp)) {
    player->up = 1;
  }
  if (event.key.code == sfKeyUp && event.type == sfEvtKeyReleased) {
    player->up = 0;
  }
}