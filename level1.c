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

#define INTERACTION_TYPE_WALK 0
#define INTERACTION_TYPE_LOOK 1
#define INTERACTION_TYPE_TALK 2

const struct timespec DELAY = {0, 90000000L};
WINDOW *textWindow, *mainWindow;

const char map1[] = ""\
"********************"\
"X             &    X"\
"X******         *  X"\
"X     *  -------*  X"\
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

void checkInteraction(struct Player *player, struct Map *map, struct Interface *interface, int interactionType){
  int playerX = round(player->x); int playerY = round(player->y);
  switch (interactionType){
    case INTERACTION_TYPE_WALK:
      if (playerX == 2 && playerY == 1) {
        addMessage(interface, "Immediately after entering, you hear a thunderous boom.");
        addMessage(interface, "The wall ceiling behind you collapse into a pile of rubble.");
        addMessage(interface, "You barely avoid the falling rock, but you are now trapped");
      }
      else if (playerX == 3 && playerY == 1)
        addMessage(interface, "The air here is thick and damp.");
      else if (playerX == 5 && playerY == 1)
        addMessage(interface, "You a light breeze. This dungeon is large and cavernous.");
      else if (playerX == 6 && playerY == 1)
        addMessage(interface, "You sense that you are not alone");
      break;
    case INTERACTION_TYPE_LOOK:
        addMessage(interface, "You are surrounded by rock and darkness. Nothing interesting here.");
      break;
    case INTERACTION_TYPE_TALK:
      addMessage(interface, "You start speaking but trail off. No one is around to hear.");
    default:
      break;
  }
}

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
  char nextPosition = getPositionInMap(map, (int) round(finalY), (int) round(finalX));
  if (nextPosition != MAP_OPEN_SPACE)
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
  struct Player player = {1, 1, 0, 0, 0.66};
  struct Interface interface = {
    textWindow,
    {"", "", ""},
    {"Tuna sandwhich", ""}
  };
  update(&player, &map, WIDTH, HEIGHT);
  updateInterface(&interface);

  while (1){
    char input = getch();
    switch(input) {
      case 'w':
        walkAnimation(&player, &map, 1, PLAYER_FORWARDS);
        checkInteraction(&player, &map, &interface, INTERACTION_TYPE_WALK);
        break;
      case 's':
        walkAnimation(&player, &map, 1, PLAYER_BACKWARDS);
        checkInteraction(&player, &map, &interface, INTERACTION_TYPE_WALK);
        break;
      case 'a':
        rotationAnimation(&player, &map, M_PI/2.0, PLAYER_COUNTER_CLOCKWISE);
        break;
      case 'd':
        rotationAnimation(&player, &map, M_PI/2.0, PLAYER_CLOCKWISE);
        break;
      case 'l':
        checkInteraction(&player, &map, &interface, INTERACTION_TYPE_LOOK);
        break;
      case 't':
        checkInteraction(&player, &map, &interface, INTERACTION_TYPE_TALK);
        break;
      default:
        break;
    }
    update(&player, &map, WIDTH, HEIGHT);
    updateInterface(&interface);
  }

  endwin();
}
