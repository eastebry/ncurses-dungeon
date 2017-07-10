#ifndef PLAYER_H_   /* Include guard */
#define PLAYER_H_

struct Player {
  double x;
  double y;
  double directionX;
  double directionY;
  double cameraPlaneX;
  double cameraPlaneY;
};

void walkForward(struct Player *player);

void walkBackwards(struct Player *player);

#endif
