/*
http://www.playfuljs.com/a-first-person-engine-in-265-lines/
http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/keys.html
https://www.viget.com/articles/game-programming-in-c-with-the-ncurses-library
*/
#include <ncurses.h>
#include <time.h>
#include <math.h>

#include "player.h"


WINDOW* main_window;
WINDOW* map_window;

const char map1[] = ""\
"**********"\
"X     &  X"\
"X        X"\
"X   $$   X"\
"X        X"\
"X        X"\
"X        X"\
"X &      X"\
"X     &  X"\
"----------";

struct Map {
  int size;
  const char *map;
};

char getPositionInMap(int y, int x, struct Map *gamemap) {
  if (y > gamemap->size || y < 0 || x < 0 || x > gamemap->size) {
    return ' ';
  }
  return gamemap->map[gamemap->size * y + x];
}

void drawColumn(int column, double colHeight, int screenHeight, char character){
  for (int i = floor(screenHeight/2.0 - colHeight/2.0); i < screenHeight/2.0 + colHeight/2.0; i++){
    mvaddch(i, column, character);
  }
}

void raycast(struct Player *player, struct Map *map, int width, int height){
  for (int x = 0; x < width; x++){
    //calculate ray position and direction
    double cameraX = 2 * x / ((double) (width)) - 1; //x-coordinate in camera space
    double rayPosX = player->x;
    double rayPosY = player->y;
    double rayDirX = cos(player->direction) + player->cameraPlaneX * cameraX;
    double rayDirY = sin(player->direction) + player->cameraPlaneY * cameraX;

    int mapX = (int) rayPosX;
    int mapY = (int) rayPosY;

    //length of ray from current position to next x or y-side
    double sideDistX;
    double sideDistY;

    //length of ray from one x or y-side to next x or y-side
    double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
    double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
    double perpWallDist;

    //what direction to step in x or y-direction (either +1 or -1)
    int stepX;
    int stepY;

    char wall = ' '  ;// the character in the map to draw the wall
    int hit = false; //was there a wall hit?
    bool outside = false; // are we outside the map?
    int side; //was a NS or a EW wall hit?

    //calculate step and initial sideDist
    if (rayDirX < 0) {
      stepX = -1;
      sideDistX = (rayPosX - mapX) * deltaDistX;
    }
    else {
      stepX = 1;
      sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
    }
    if (rayDirY < 0) {
      stepY = -1;
      sideDistY = (rayPosY - mapY) * deltaDistY;
    }
    else {
      stepY = 1;
      sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
    }
    //perform DDA
    while (!hit && !outside) {
      //jump to next map square, OR in x-direction, OR in y-direction
      if (sideDistX < sideDistY) {
        sideDistX += deltaDistX;
        mapX += stepX;
        side = 0;
      }
      else {
        sideDistY += deltaDistY;
        mapY += stepY;
        side = 1;
      }
      if (mapX < 0 || mapY < 0 || mapY > map->size || mapY > map->size) {
        outside = true;
      }
      //Check if ray has hit a wall
      wall = getPositionInMap(mapY, mapX, map);
      if (wall != ' ') hit = true;
    }

    //Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
    if (hit) {
      if (side == 0) perpWallDist = (mapX - rayPosX + (1 - stepX) / 2) / rayDirX;
      else           perpWallDist = (mapY - rayPosY + (1 - stepY) / 2) / rayDirY;

      //Calculate height of line to draw on screen
      int lineHeight = (int)(height / perpWallDist);
      drawColumn(x, lineHeight, height, wall);
    }
  }
}

void drawMiniMap(struct Player *player, struct Map *map){
  int offset = 250;
  for (int i = 0; i < map->size; i++){
    for (int j = 0; j < map->size; j++){
      mvaddch(j, i + offset, getPositionInMap(j,i, map));
    }
  }
  mvaddch(player->y, player->x + offset, 'P');
  char str1[80];
  sprintf(str1, "X: %f", player->x);
  mvprintw(10, offset, &str1);
  char str2[80];
  sprintf(str2, "Y: %f", player->y);
  mvprintw(15, offset, &str2);
  char str3[80];
  sprintf(str3, "Direction: %f", player->direction);
  mvprintw(20, offset, &str3);
  char str4[80];
  sprintf(str4, "Time: %d", (int)time(NULL));
  mvprintw(30, offset, &str4);
}

void update(struct Player *player, struct Map *map, int w, int h){
  clear();
  raycast(player, map, w, h);
  drawMiniMap(player, map);
  refresh();
}

void walkAnimation(struct Player *player, struct Map *map, double distance, double direction){
  const struct timespec delay = {0, 90000000L};
  double moveSpeed = .1;
  double finalX = player-> x + cos(player->direction) * distance * direction;
  double finalY = player-> y + sin(player->direction) * distance * direction;
  for (double i = 0; i <(int) floor(distance/moveSpeed); i+=1){
    walk(player, moveSpeed, direction);
    update(player, map, 200, 60);
    nanosleep(&delay, NULL);
  }
  player->x = finalX;
  player->y = finalY;
}

void rotationAnimation(struct Player *player, struct Map *map, double radians, int direction){
  double moveSpeed = 0.1;
  const struct timespec delay = {0, 90000000L};
  double finalDirection = player->direction + (radians * direction);
  //make sure to keep the camera plane up to date. It needs to be perpendicular to the direction
  double finalCameraX = player->cameraPlaneX * cos(radians * direction) - player->cameraPlaneY * sin(radians * direction);
  double finalCameraY = player->cameraPlaneX * sin(radians * direction) +  player->cameraPlaneY * cos(radians * direction);

  for (double i=0; i < (int) floor(fabs(radians/moveSpeed)); i+=1){
    rotate(player, moveSpeed, direction);
    // TODO -factor this call to update out somehow
    update(player, map, 200, 60);
    nanosleep(&delay, NULL);
  }
  player->direction = finalDirection;
  player->cameraPlaneX = finalCameraX;
  player->cameraPlaneY = finalCameraY;
}

int main(){

  initscr();
  cbreak();
  noecho();
  //nodelay(stdscr, TRUE);
  curs_set(FALSE);

  struct Map map = {10, &map1};
  struct Player player = {5, 5, 0, 0, 0.66};
  update(&player, &map, 200, 60);

  while (1){
    char input = getch();
    switch(input) {
      case 'w':
      walkAnimation(&player, &map, 1, PLAYER_FORWARDS);
      break;
      case 's':
      walkAnimation(&player, &map, 1, PLAYER_BACKWARDS);
      break;
      case 'a':
      rotationAnimation(&player, &map, M_PI/2.0, PLAYER_COUNTER_CLOCKWISE);
      break;
      case 'd':
      rotationAnimation(&player, &map, M_PI/2.0, PLAYER_CLOCKWISE);
      break;
      default:
      break;
    }
    update(&player, &map, 200, 60);
  }

  endwin();
}
