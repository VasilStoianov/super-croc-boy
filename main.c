#include "SFML/Graphics.h"
#include "SFML/System.h"
#include "SFML/Window.h"
#include "game.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>

int main() {

  sfRectangleShape *rect = sfRectangleShape_create();

  sfRectangleShape_setSize(rect, (sfVector2f){300.f, 300.f});
  sfRectangleShape_setPosition(rect, (sfVector2f){0.0f, 724.0f});
  sfRectangleShape_setFillColor(rect, sfRed);
  sfRenderStates state = sfRenderStates_default();

  sfEvent event;
  Player *player = createPlayer();

  loadSprites(player);
  player->size.x *= 0.125f;
  player->size.y *= 0.125f;
  size_t debug = 0;

  int index = hash("jump") % animations;
  int animFrames = player->frames[index];
  size_t frame = 0;

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
sfRectangleShape_setFillColor(spriteBox,sfBlue);
sfRectangleShape_setSize(spriteBox,player->size);
sfRectangleShape_setOutlineColor(spriteBox,sfWhite);
sfRectangleShape_setOutlineThickness(spriteBox,2.f);  

  while (sfRenderWindow_isOpen(renderer)) {
    elapsedTime = sfClock_getElapsedTime(time);
    timeAsSeconds = sfTime_asMilliseconds(elapsedTime);

    sfRenderWindow_clear(renderer, sfBlack);

    while (sfRenderWindow_pollEvent(renderer, &event)) {
      if (event.key.type == sfEvtClosed) {
        sfRenderWindow_close(renderer);
      }
      if (event.key.code == sfKeyEscape) {
        sfRenderWindow_close(renderer);
      }
      if(event.key.code  == sfKeyD && event.type == sfEvtKeyPressed){
        debug = !debug;       
      }
      if(event.key.code == sfKeyRight && event.type == sfEvtKeyPressed){
        player->velocity.x += 1.f * dt;
        index = hash("run") % animations;
       animFrames = player->frames[index];
      }
     if(event.key.code == sfKeyLeft && event.type == sfEvtKeyPressed){
        player->velocity.x -= 100.f * dt;
      }
 if(event.key.code == sfKeyRight && event.type == sfEvtKeyReleased){
       
  player->velocity.x = 0;
   index = hash("idle") % animations;
   frame = 0;
       animFrames = player->frames[index]; 
}
     if(event.key.code == sfKeyLeft && event.type == sfEvtKeyReleased){
        player->velocity.x = 0;
      }

    }
 
    applyGravity(&player);
    player->position.x += player->velocity.x;
    sfRenderWindow_drawRectangleShape(renderer, rect, &state);
    sfSprite * sprite = player->sprites[index][frame];
    sfSprite_setPosition(sprite, player->position);
    if(debug){

    sfRenderWindow_drawRectangleShape(renderer,spriteBox,&state);
    }
    sfRenderWindow_drawSprite(renderer, player->sprites[index][frame], &state);
    sfVector2f ox = aabb(*player, rect);
    if(ox.x > 0 && ox.y > 0){
      if(player->velocity.x == 0){
       index = hash("idle") % animations;
       frame = 0;
       animFrames = player->frames[index];
      }
     player->velocity.y = 0;
    //  ox.y += .220f;
     player->position.y -= ox.y * dt ;
     player->position.y -= .220f;
    }
   
sfRectangleShape_setPosition(spriteBox,player->position);
    if (timeAsSeconds > 155) {
      frame++;
      if (frame >= animFrames-1)
        frame = 0;
      sfClock_restart(time);
      timeAsSeconds = sfTime_asSeconds(elapsedTime);
    }

    sfRenderWindow_display(renderer);
  }

  sfClock_destroy(time);
  spriteDestroy(player);
  free(player);

  return 1;
}