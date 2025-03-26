#pragma once
#include "player.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

const float GRAVITY = 9.81;
const float dt = 1.f / 60.f;

void applyGravity(Player **p) {
  // (*p)->velocity.y += (GRAVITY /  (*p)->mass) * dt;
  // (*p)->position.y += ((*p)->velocity.y * dt ) *12500;
  (*p)->mass += 15;

  (*p)->velocity.y += (*p)->mass;
  (*p)->position.y += (*p)->velocity.y * dt;
}

sfVector2f aabb(sfSprite *sprite, sfRectangleShape *rect) {

  sfFloatRect rectPos = sfRectangleShape_getGlobalBounds(rect);
  sfVector2f position = sfRectangleShape_getPosition(rect);
  sfVector2f rectCenter;
  rectCenter.x = rectPos.left + rectPos.width / 2;
  rectCenter.y = rectPos.top + rectPos.height / 2;

  sfFloatRect spritePos = sfSprite_getGlobalBounds(sprite);
  sfVector2f pCenter;
  pCenter.x = (spritePos.left + spritePos.width / 2);
  pCenter.y = (spritePos.top + spritePos.height / 2);

  sfVector2f delt = (sfVector2f){.x = abs(pCenter.x - rectCenter.x),
                                 .y = abs(pCenter.y - rectCenter.y)};
  float ox = (spritePos.width) / 2 + (rectPos.width) / 2 - delt.x;
  float oy = (spritePos.height) / 2 + (rectPos.height) / 2 - delt.y;

  sfVector2f res = (sfVector2f){.x = ox, .y = oy};
  return res;
}

void handleCollision(Player *player, sfSprite *sprite, sfRectangleShape *rect) {

  sfVector2f ox = aabb(sprite, rect);
  if (ox.x > 0 && ox.y > 0) {

    sfFloatRect playerOrigin = sfSprite_getGlobalBounds(sprite);
    sfFloatRect rectOrigin = sfRectangleShape_getGlobalBounds(rect);
    if (playerOrigin.top + playerOrigin.height > rectOrigin.top && playerOrigin.top < rectOrigin.top + rectOrigin.height) {
      player->velocity.y = 0;
      player->mass = 0;
      player->position.y = rectOrigin.top - playerOrigin.height / 2;
      player->inAir = 0;
    }
    if (player->previousPosition.x < player->position.x) {
      player->position.x = rectOrigin.left - rectOrigin.width;
    }
    if (player->position.x > rectOrigin.left + rectOrigin.width) {
      printf("Right\n");
    }
    if (player->velocity.x == 0) {
      player->index = hash("idle") % animations;
      player->frame = 0;
      player->animFrames = player->frames[player->index];
    }
  }
}