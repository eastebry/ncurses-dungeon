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
      double rayDirX = player->directionX + player->cameraPlaneX * cameraX;
      double rayDirY = player->directionY + player->cameraPlaneY * cameraX;

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
     int hit = 0; //was there a wall hit?
     int side; //was a NS or a EW wall hit?

     //calculate step and initial sideDist
     if (rayDirX < 0)
     {
       stepX = -1;
       sideDistX = (rayPosX - mapX) * deltaDistX;
     }
     else
     {
       stepX = 1;
       sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
     }
     if (rayDirY < 0)
     {
       stepY = -1;
       sideDistY = (rayPosY - mapY) * deltaDistY;
     }
     else
     {
       stepY = 1;
       sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
     }
     //perform DDA
     while (hit == 0)
     {
       //jump to next map square, OR in x-direction, OR in y-direction
       if (sideDistX < sideDistY)
       {
         sideDistX += deltaDistX;
         mapX += stepX;
         side = 0;
       }
       else
       {
         sideDistY += deltaDistY;
         mapY += stepY;
         side = 1;
       }
       //Check if ray has hit a wall
       wall = getPositionInMap(mapY, mapX, map);
       if (wall != ' ') hit = 1;
     }
     //Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
     if (side == 0) perpWallDist = (mapX - rayPosX + (1 - stepX) / 2) / rayDirX;
     else           perpWallDist = (mapY - rayPosY + (1 - stepY) / 2) / rayDirY;

     //Calculate height of line to draw on screen
     int lineHeight = (int)(height / perpWallDist);
     drawColumn(x, lineHeight, height, wall);
   }
 }

void drawMiniMap(struct Map *map, int playerX, int playerY){
  int offset = 250;
  for (int i = 0; i < map->size; i++){
    for (int j = 0; j < map->size; j++){
      mvaddch(j, i + offset, getPositionInMap(j,i, map));
    }
  }
  mvaddch(playerY, playerX + offset, 'P');
}

void update(struct Player *player, struct Map *map, int w, int h){
    clear();
    raycast(player, map, w, h);
    drawMiniMap(map, player->x, player->y);
    refresh();
}

int main(){

  initscr();
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);
  curs_set(FALSE);
  const struct timespec delay = {0, 90000000L};

  struct Player player = {5, 5, -1, 0, 0, 0.66};
  struct Map map = {10, &map1};

  while (1){
    //rotateRight(&player);
    update(&player, &map, 200, 60);
    char input = getch();
    switch(input) {
      case KEY_UP:
      case 'w':
        walkForward(&player);
        break;
      case KEY_DOWN:
      case 's':
        walkBackwards(&player);
        break;
      default:
        break;
    }
    nanosleep(&delay, NULL);
  }

  endwin();
}
