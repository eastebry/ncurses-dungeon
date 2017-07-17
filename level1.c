#include <ncurses.h>
#include <time.h>
#include <math.h>

#include "raycaster.h"
#include "player.h"
#include "map.h"
#include "interface.h"

// Screen width and height
#define WIDTH 150
#define HEIGHT 50


const struct timespec DELAY = {0, 90000000L};
WINDOW *textWindow, *mainWindow;

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

void update(struct Player *player, struct Map *map, int w, int h){
  //wclear(mainWindow);
  //TODO is there a way to get this to work?
  werase(mainWindow);
  raycast(player, map, mainWindow, w, h);
  //drawMiniMap(player, map);
  wrefresh(mainWindow);
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

int main(){
  //resizeterm(WIDTH, HEIGHT);
  initscr();
  start_color();
  initColors();
  cbreak();
  noecho();
  //nodelay(stdscr, TRUE);
  curs_set(FALSE);

  mainWindow = newwin(HEIGHT, WIDTH, 0, 0);
  textWindow = newwin(6, WIDTH, HEIGHT, 0);

  struct Map map = {20, &map1};
  struct Player player = {2, 2, 0, 0, 0.66};
  struct Interface interface = {
    textWindow,
    {"", "", ""},
    {"Tuna sandwhich", "<empty>"}
  };
  update(&player, &map, WIDTH, HEIGHT);
  updateInterface(&interface);

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
      case 'l':
        addMessage(&interface, "The air is cold and damp");
        break;
      case 't':
        addMessage(&interface, "You start speaking but trail off. No one is around to hear.");
        break;
      default:
        break;
    }
    update(&player, &map, WIDTH, HEIGHT);
    updateInterface(&interface);
  }

  endwin();
}
