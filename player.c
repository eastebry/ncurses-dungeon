#include <math.h>
#include "player.h"

void rotateRight(struct Player *player){

  /*
  This code rotates in intermediate directions
  double rotSpeed = .005;
  double oldDirX = player->directionX;
  player->directionX = player->directionX * cos(-rotSpeed) - player->directionY * sin(-rotSpeed);
  player->directionY = oldDirX * sin(-rotSpeed) + player->directionY * cos(-rotSpeed);
  double oldPlaneX = player->cameraPlaneX;
  player->cameraPlaneX = player->cameraPlaneX * cos(-rotSpeed) - player->cameraPlaneY * sin(-rotSpeed);
  player->cameraPlaneY = oldPlaneX * sin(-rotSpeed) + player->cameraPlaneY * cos(-rotSpeed);
  */

  // for now just rotate 90 degress

}

void walk(struct Player *player, int direction){
  player->x += cos(player->direction) * direction;
  player->y += sin(player->direction) * direction;
}

void rotate(struct Player *player, int direction){
  // for now we are just rotating at 90 degree angles
  player->direction += (M_PI/2.0) * direction;
}
