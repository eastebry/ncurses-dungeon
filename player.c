#include <math.h>
#include "player.h"

void rotateRight(struct Player *player){
  /*
  double rotSpeed = .005;
  double oldDirX = player->directionX;
  player->directionX = player->directionX * cos(-rotSpeed) - player->directionY * sin(-rotSpeed);
  player->directionY = oldDirX * sin(-rotSpeed) + player->directionY * cos(-rotSpeed);
  double oldPlaneX = player->cameraPlaneX;
  player->cameraPlaneX = player->cameraPlaneX * cos(-rotSpeed) - player->cameraPlaneY * sin(-rotSpeed);
  player->cameraPlaneY = oldPlaneX * sin(-rotSpeed) + player->cameraPlaneY * cos(-rotSpeed);
  */
}

void walkForward(struct Player *player){
  player->x = player->x + (player->directionX);
  player->y = player->y + (player->directionY);
}

void walkBackwards(struct Player *player){
  player->x = player->x - (player->directionX);
  player->y = player->y - (player->directionY);
}
