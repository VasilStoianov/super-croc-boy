#include "SFML/Graphics.h"
#include "SFML/System.h"
#include "SFML/Window.h"
#include "game.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>

int main() {

  sfRectangleShape **rects = malloc(4 * sizeof(sfRectangleShape **));
  int rectNum[4];
  for (int x = 0; x < 5; x++) {

    sfRectangleShape *rect = sfRectangleShape_create();
    sfRectangleShape_setSize(rect, (sfVector2f){300.f, 300.f});
    if (x == 4) {

      sfRectangleShape_setPosition(rect, (sfVector2f){0.0f + 300.f * 2, 300.f});
    } else {

      sfRectangleShape_setPosition(rect,
                                   (sfVector2f){0.0f + 300.f * x, 724.0f});
    }

    sfRectangleShape_setFillColor(rect, sfRed);
    rects[x] = malloc(sizeof(sfRectangleShape *));
    rects[x] = rect;
    rectNum[x] = x;
  }

  sfRenderStates state = sfRenderStates_default();

  sfEvent event;
  Player *player = createPlayer();

  loadSprites(player);
  player->size.x *= 0.125f;
  player->size.y *= 0.125f;
  size_t debug = 0;

  player->index = hash("jump") % animations;
  player->animFrames = player->frames[player->index];
  player->frame = 0;

  sfClock *time = sfClock_create();
  sfTime startTime = sfClock_restart(time);
  sfTime elapsedTime = sfClock_getElapsedTime(time);
  float timeAsSeconds = sfTime_asSeconds(elapsedTime);

  sfRenderWindow *renderer = sfRenderWindow_create(
      (sfVideoMode){.height = 1024, .width = 1024, .bitsPerPixel = 32},
      "Super Croc Boy", sfResize | sfClose, NULL);

  if (!renderer)
    return EXIT_FAILURE;

  sfRenderWindow_setFramerateLimit(renderer, 60);

  sfRectangleShape *spriteBox = sfRectangleShape_create();
  sfRectangleShape_setFillColor(spriteBox, sfBlue);
  sfRectangleShape_setSize(spriteBox, player->size);
  sfRectangleShape_setOutlineColor(spriteBox, sfWhite);
  sfRectangleShape_setOutlineThickness(spriteBox, 2.f);
  sfFloatRect spriteBoxOrigin = sfRectangleShape_getGlobalBounds(spriteBox);
  sfRectangleShape_setOrigin(
      spriteBox,
      (sfVector2f){.x = spriteBoxOrigin.left + spriteBoxOrigin.width / 2,
                   .y = spriteBoxOrigin.top + spriteBoxOrigin.height / 2});
  while (sfRenderWindow_isOpen(renderer)) {
    elapsedTime = sfClock_getElapsedTime(time);
    timeAsSeconds = sfTime_asMilliseconds(elapsedTime);

    sfRenderWindow_clear(renderer, sfBlack);

    while (sfRenderWindow_pollEvent(renderer, &event)) {
      handleKeyInput(event,renderer,player);
    }

    sfVector2i mPos = sfMouse_getPositionRenderWindow(renderer);
    // player->position.x = mPos.x;
    // player->position.y = mPos.y;

    sfSprite *sprite = player->sprites[player->index][player->frame];
    sfSprite_setScale(sprite, player->scale);

    applyGravity(&player);
    for (int x = 0; x < 5; x++) {
      sfRenderWindow_drawRectangleShape(renderer, rects[x], &state);
      handleCollision(player, sprite, rects[x]);
    }

    sfSprite_setPosition(sprite, player->position);
    if (debug) {
      sfRenderWindow_drawRectangleShape(renderer, spriteBox, &state);
    }
    sfRenderWindow_drawSprite(
        renderer, player->sprites[player->index][player->frame], &state);

    updatePlayer(player);
    sfRectangleShape_setPosition(spriteBox, player->position);

    if (timeAsSeconds > 750 / player->animFrames) {
      player->frame++;
      if (player->frame >= player->animFrames)
        player->frame = 0;
      sfClock_restart(time);
      timeAsSeconds = sfTime_asSeconds(elapsedTime);
    }

    sfRenderWindow_display(renderer);
    player->previousPosition = player->position;
  }

  sfClock_destroy(time);
  spriteDestroy(player);
  free(player);

  return 1;
}