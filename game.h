#pragma once 
#include <stdlib.h>
#include <stdio.h>
#include "player.h"
#include <math.h>

const float GRAVITY = 9.81;
const float dt = 1.f/60.f;

void applyGravity(Player **p){
  
    (*p)->velocity.y += (GRAVITY /  (*p)->mass) * dt;
    (*p)->position.y += ((*p)->velocity.y * dt ) *12500; 
}

sfVector2f aabb(Player p, sfRectangleShape* rect){

  sfFloatRect rectPos =   sfRectangleShape_getGlobalBounds(rect);
  sfVector2f position =sfRectangleShape_getPosition(rect);
  sfVector2f rectCenter;
  rectCenter.x = rectPos.left + rectPos.width/2;
  rectCenter.y = rectPos.top + rectPos.height/2;

  sfVector2f pCenter;
  pCenter.x = (p.position.x + p.size.x /2) ;
  pCenter.y = (p.position.y + p.size.y/2);

  sfVector2f delt = (sfVector2f){.x = abs(pCenter.x - rectCenter.x),
    .y = abs(pCenter.y - rectCenter.y)};
  float ox =(p.size.x)/2 + (rectPos.width)/2 - delt.x;
  float oy = (p.size.y )/2 + (rectPos.height)/2 - delt.y;
   
 sfVector2f res = (sfVector2f){.x = ox,.y=oy};
   return res;

}