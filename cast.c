#include <ncurses.h>
#include <time.h>
#include <math.h>

#include "player.h"

// Screen width and height
#define WIDTH 150
#define HEIGHT 50

// Define the color indexes
#define WALL_RED 1
#define WALL_GREEN 2
#define WALL_YELLOW 3
#define WALL_CYAN 4
#define FLOOR 5
#define BLACK 6

// shorthand for color pairs
#define ATTR_ALL_BLACK COLOR_PAIR(BLACK)
#define ATTR_WALL_RED COLOR_PAIR(WALL_RED)
#define ATTR_WALL_GREEN COLOR_PAIR(WALL_GREEN)
#define ATTR_WALL_YELLOW COLOR_PAIR(WALL_YELLOW)
#define ATTR_WALL_CYAN COLOR_PAIR(WALL_CYAN)
#define ATTR_FLOOR COLOR_PAIR(FLOOR)

const struct timespec DELAY = {0, 90000000L};

const char map1[] = ""\
"********************"\
"X             &    X"\
"X     *   X     *  X"\
"X     *---------*  X"\
"X               *& X"\
"X------  XXXX  X*  X"\
"X     *  X      *  X"\
"X  &  *  X&&&&&&&  X"\
"X  &               X"\
"X  &&&&  ***  ***  X"\
"X   &    X     &X  X"\
"X** &  * X &&& &   X"\
"-      *   &      XX"\
"-   ------------FF--"\
"X         XXX**    X"\
"X         XXX**    X"\
"X            ******X"\
"X****&&&           X"\
"X   *       &&&&&&&X"\
"X   *    *  -      X"\
"--------------------";

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

int getWallColor(char character){
  if (character == '*')
    return ATTR_WALL_RED;
  if (character == 'X')
    return ATTR_WALL_GREEN;
  if (character == '-')
    return ATTR_WALL_YELLOW;
  if (character == '&')
    return ATTR_WALL_CYAN;
  return ATTR_WALL_YELLOW;
}

void drawColumn(int column, double colHeight, int screenHeight, char character){
  int color = getWallColor(character);
  int colTop = (int) floor(screenHeight/2.0 - colHeight/2.0);
  int colBottom = (int) floor(screenHeight/2.0 + colHeight/2.0);
  attron(color);
  for (int i = colTop; i < screenHeight; i++){
    if (i == colBottom) {
      attroff(color);
      color = ATTR_FLOOR;
      character = '.';
      attron(color);
    }
    mvaddch(i, column, character);
  }
  attroff(color);
}

void raycast(struct Player *player, struct Map *map, int width, int height){
  for (int x = 0; x < width; x++){
    //calculate ray position and direction
    double cameraX = 2 * x / ((double) (width)) - 1; //x-coordinate in camera space
    // cast rays from the middle of the cell
    double rayPosX = player->x + .5;
    double rayPosY = player->y + .5;
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

    char wall = ' '; // the character in the map to draw the wall
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
  int offset = 150;
  for (int i = 0; i < map->size; i++){
    for (int j = 0; j < map->size; j++){
      mvaddch(j, i + offset, getPositionInMap(j,i, map));
    }
  }
  mvaddch((int)round(player->y), ((int) round(player->x)) + offset, 'P');
  char str1[80];
  sprintf(str1, "X: %9.7f", player->x);
  mvprintw(25, offset, &str1);
  char str2[80];
  sprintf(str2, "Y: %9.7f", player->y);
  mvprintw(26, offset, &str2);
  char str3[80];
  sprintf(str3, "Direction: %9.7f", player->direction);
  mvprintw(27, offset, &str3);
  char str4[80];
  sprintf(str4, "X (int): %d", (int) round(player->x));
  mvprintw(28, offset, &str4);
  char str5[80];
  sprintf(str5, "Y (int): %d", (int) round(player->y));
  mvprintw(29, offset, &str5);

}


void update(struct Player *player, struct Map *map, int w, int h){
  clear();
  //erase();
  raycast(player, map, w, h);
  drawMiniMap(player, map);
  attron(A_BOLD);
  box(stdscr, 0, 0);
  attroff(A_BOLD);
  refresh();
}

void walkAnimation(struct Player *player, struct Map *map, double distance, double direction){
  double moveSpeed = .1;
  double finalX = player-> x + cos(player->direction) * distance * direction;
  double finalY = player-> y + sin(player->direction) * distance * direction;
  if (getPositionInMap((int) round(finalY), (int) round(finalX), map) != ' ')
    return;
  for (double i = 0; i <(int) floor(distance/moveSpeed); i+=1){
    walk(player, moveSpeed, direction);
    update(player, map, WIDTH, HEIGHT);
    nanosleep(&DELAY, NULL);
  }
  player->x = finalX;
  player->y = finalY;
}

void rotationAnimation(struct Player *player, struct Map *map, double radians, int direction){
  double moveSpeed = 0.15;
  double finalDirection = player->direction + (radians * direction);
  //make sure to keep the camera plane up to date. It needs to be perpendicular to the direction
  double finalCameraX = player->cameraPlaneX * cos(radians * direction) - player->cameraPlaneY * sin(radians * direction);
  double finalCameraY = player->cameraPlaneX * sin(radians * direction) +  player->cameraPlaneY * cos(radians * direction);

  for (double i=0; i < (int) floor(fabs(radians/moveSpeed)); i+=1){
    rotate(player, moveSpeed, direction);
    // TODO -factor this call to update out somehow
    update(player, map, WIDTH, HEIGHT);
    nanosleep(&DELAY, NULL);
  }
  player->direction = finalDirection;
  player->cameraPlaneX = finalCameraX;
  player->cameraPlaneY = finalCameraY;
}

void initColors(){
  init_pair(WALL_RED, COLOR_RED, COLOR_RED);
  init_pair(WALL_GREEN, COLOR_GREEN, COLOR_GREEN);
  init_pair(WALL_YELLOW, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(WALL_CYAN, COLOR_CYAN, COLOR_CYAN);
  init_pair(FLOOR, COLOR_BLUE, COLOR_BLACK);
  init_pair(BLACK, COLOR_BLACK, COLOR_BLACK);
}
int main(){

  resizeterm(WIDTH, HEIGHT);
  initscr();
  cbreak();
  noecho();
  //nodelay(stdscr, TRUE);
  curs_set(FALSE);
  start_color();
  initColors();

  struct Map map = {20, &map1};
  struct Player player = {2, 2, 0, 0, 0.66};
  update(&player, &map, WIDTH, HEIGHT);

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
    update(&player, &map, WIDTH, HEIGHT);
  }

  endwin();
}
