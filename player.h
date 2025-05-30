#pragma once
#include "SFML/Graphics.h"
#include "SFML/System.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern float dt;

const int animations = 4;


typedef struct Player {
  sfVector2f position;
  sfVector2f velocity;
  sfVector2f previousPosition;
  int playerAcceleration;
  sfVector2f scale;
  sfSprite ***sprites;
  sfVector2f size;
 sfBool up;
 sfBool down;
 sfBool left;
 sfBool right;
  int frames[4];
  int animationLoaded;
  float mass;
  int inAir;
  int index ;
  int animFrames;
  int maxYvelocity;
  size_t frame;
} Player;

void spriteDestroy(Player *player);
void spriteDestroy(Player *player) {

  for (int x = 0; x < 4;x++) {
    int frames = player->frames[x];
    for (int y = 0; y < frames; y++) {
      sfTexture_destroy(sfSprite_getTexture(player->sprites[x][y]));
      free(player->sprites[x][y]);
    }
    free(player->sprites[x]);
  }
}

int hash(char *string) {

  int num = 0;
  int len = strlen(string);
  for (int x = 0; x < len; x++) {

    num += string[x] + x;
  }

  return num + len;
}

void loadSprite(Player **player, char *path, int frames,char * key) {
    if((*player)->animationLoaded){
    (*player)->sprites = (sfSprite***) malloc(animations * sizeof(sfSprite ***));
    (*player)->animationLoaded = 0;
    }
    if (!(*player)->sprites) {
      printf("ERROR: Cannot allocate sprites\n");
    }
  

  int index = hash(key) % animations;
  (*player)->frames[index] = frames;
  (*player)->sprites[index] = malloc(frames * sizeof(sfSprite**));
  
  for (int x = 1; x <= frames; x++) {
    char *tempPath;
    int length = strlen(path) + snprintf(NULL, 0, "%d", x) + 5;
    tempPath = (char *)malloc(length);
    strcpy(tempPath, path);
    snprintf(tempPath + strlen(path), length - strlen(tempPath), "%d.png", x);
    printf("INFO: Loaded sprite:  %s\n", tempPath);

    (*player)->size = (sfVector2f){.x = 290.f,.y=409.f};

    sfTexture *text = sfTexture_createFromFile(
        tempPath,
        &(sfIntRect){
            .top = 0.f, .left = 25.f, .width = (*player)->size.x, .height =(*player)->size.y});
     (*player)->sprites[index][x - 1]  = sfSprite_create();
    sfSprite_setTexture ((*player)->sprites[index][x - 1], text, 1);
    
    (*player)->scale = (sfVector2f){.x = 0.125, .y = 0.125};
     sfFloatRect or = sfSprite_getGlobalBounds((*player)->sprites[index][x - 1]);
    sfVector2f orrr = (sfVector2f){.x = or.left + or.width/2 , .y = or.top + or.height/2};
    sfSprite_setOrigin((*player)->sprites[index][x - 1],orrr);
    sfSprite_setScale   ((*player)->sprites[index][x - 1],(*player)->scale);
    sfSprite_setPosition((*player)->sprites[index][x - 1], (sfVector2f){125.f, 125.f});
    free(tempPath);
  }
}

void loadSprites(Player * player){
  char * idle = "./textures/idle/frame-";
  char * dizzy = "./textures/dizzy/frame-";
  char * path = "./textures/run/frame-";
  char * jumpFall = "./textures/jump fall/frame-";
  loadSprite(&player,idle,4,"idle");
  loadSprite(&player,dizzy,2,"dizzy");
  loadSprite(&player,path,4,"run");
  loadSprite(&player,jumpFall,1,"jump");
}

Player* createPlayer(){
  Player *player = malloc(sizeof(Player));
  player->velocity = (sfVector2f){.x = 0,.y=0};
  player->position = (sfVector2f){.x = 150,.y=0};
  player->previousPosition = player->position;
  player->mass = 25;
  player->maxYvelocity = 10;
  player->animationLoaded = 1;
  player->inAir = 1;
  player->playerAcceleration = 3;
 return player;
}

void playerJump(Player * player){
  printf("Jumping\n");
  player->mass = -10;
  // player->position.y += -15;
  player->inAir = 1;
}