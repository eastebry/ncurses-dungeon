#include <ncurses.h>
#include <time.h>
#include <math.h>

#include "raycaster.h"
#include "player.h"
#include "map.h"

// Screen width and height
#define WIDTH 150
#define HEIGHT 50


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


void drawMiniMap(struct Player *player, struct Map *map){
  int offset = 150;
  for (int i = 0; i < map->size; i++){
    for (int j = 0; j < map->size; j++){
      mvaddch(j, i + offset, getPositionInMap(map, j, i));
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
  if (getPositionInMap(map, (int) round(finalY), (int) round(finalX)) != MAP_OPEN_SPACE)
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
    update(player, map, WIDTH, HEIGHT);
    nanosleep(&DELAY, NULL);
  }
  player->direction = finalDirection;
  player->cameraPlaneX = finalCameraX;
  player->cameraPlaneY = finalCameraY;
}

void initColors() {
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
