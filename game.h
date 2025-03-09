#pragma once 
#include <stdlib.h>
#include <stdio.h>
#include "player.h"
#include <math.h>

const float GRAVITY = 9.81;
float dt = 1.f/60.f;

void applyGravity(Player **p){
  
    sfVector2f square = (sfVector2f){.x = 50.f,.y=400};
    // float distance = sqrtf(((square.x - p->position.x) * (square.x - p->position.x) + (square.y - p->position.y) * (square.y - p->position.y)));
    (*p)->velocity.y += (GRAVITY /  (*p)->mass) * dt;
    (*p)->position.y += ((*p)->velocity.y * dt ) *1000; 
    printf("Velocity y: %f\n",(*p)->position.y*1000);

}