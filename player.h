#pragma once
#include "SFML/Graphics.h"
#include "SFML/System.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Player {
  sfVector2f position;
  sfVector2f velocity;
  sfSprite ***sprites;
  int frames[3];
  int animationLoaded;
  float mass;
} Player;

int hash(char *string) {

  int num = 0;
  int len = strlen(string);
  for (int x = 0; x < len; x++) {

    num += string[x] + x;
  }

  return num + len;
}

void loadSprite(Player **player, char *path, int frames,char * key) {
    int animations = 3;
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

    sfTexture *text = sfTexture_createFromFile(
        tempPath,
        &(sfIntRect){
            .top = 0.f, .left = 25.f, .width = 290.f, .height = 409.f});
     (*player)->sprites[index][x - 1]  = sfSprite_create();
    sfSprite_setTexture ((*player)->sprites[index][x - 1], text, 1);
    sfSprite_setPosition((*player)->sprites[index][x - 1], (sfVector2f){125.f, 125.f});
    sfSprite_setScale   ((*player)->sprites[index][x - 1], (sfVector2f){.x = 0.125, .y = 0.125});
    // sfTexture_destroy(text);
    free(tempPath);
  }
}

void loadSprites(Player * player){

  char * idle = "./textures/idle/frame-";
  char * dizzy = "./textures/dizzy/frame-";
  char * path = "./textures/run/frame-";
  loadSprite(&player,idle,2,"idle");
  loadSprite(&player,dizzy,2,"dizzy");
  loadSprite(&player,path,4,"run");

}

Player* createPlayer(){

  Player *player = malloc(sizeof(Player));
  player->velocity = (sfVector2f){.x = 0,.y=0};
  player->mass = 65;
  player->animationLoaded = 1;
 return player;

}