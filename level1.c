#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "engine.h"
#include "raycaster.h"
#include "player.h"
#include "map.h"
#include "interface.h"

// Screen width and height
#define WIDTH 150
#define HEIGHT 50


const struct timespec DELAY = {0, 90000000L};
WINDOW *textWindow, *mainWindow;

const int mapSize = 20;
const char map1[] = ""\
"********************"\
"X ab  c dd&     *  X"\
"X*** **  d&     *  X"\
"Xe  --*  -------*  X"\
"Xee             *-&X"\
"X------- XXXX  X*  X"\
"X     **pX X    *  X"\
"X  &  ** X&&&&&&&  X"\
"X  &              -X"\
"X  ---&&&& *****   X"\
"X        X     &X  X"\
"X** &  * XX&&& &   X"\
"-   &      &       X"\
"-   ------------F---"\
"X         XXX**    X"\
"X    -    X   *    X"\
"X         XF-F--F-*X"\
"X****&&&&&  &    XXX"\
"X    F&&---F---F---X"\
"X   &FFFF    -   --X"\
"--------------------";

void checkInteraction(struct Player *player, struct Map *map, struct Interface *interface, int interactionType){
  clearMessage(interface);
  char marker = getPositionInMap(map, player->y, player->x);
  switch (interactionType){
    case INTERACTION_TYPE_WALK:
      if (marker == 'a'){
        addMessage(interface, "Immediately after entering, you hear a thunderous boom");
        addMessage(interface, "The wall ceiling behind you collapse into a pile of rubble");
        addMessage(interface, "You barely avoid the falling rock, but you are now trapped");
      }
      else if (marker == 'b')
        addMessage(interface, "The air here is thick and damp.");
      else if (marker == 'c')
        addMessage(interface, "You feel a light breeze. This dungeon is large and cavernous.");
      else if (marker == 'd'){
        addMessage(interface, "There is a skeleton lying on the ground. It has clearly been here for while.");
        addMessage(interface, "The bones of its fingers are curled around a piece of parchment.");
        addMessage(interface, "Press (L) to read the note.");
      }
      else if (marker == 'e'){
        addMessage(interface, "There is a another skeleton on the ground.");
        addMessage(interface, "There some paper lying on the ground next to it.");
      }
      else if (marker == 'p') {
        addMessage(interface, "There's an object lying on the ground.");
      }
      break;
    case INTERACTION_TYPE_LOOK:
      if (marker == 'd') {
        addMessage(interface, "The note reads: I've been trapped in this maze for weeks.");
        addMessage(interface, "This dungeon is a labyrenth. I've expored every passageway ten times,");
        addMessage(interface, "but still can't find where I entered. I wish I had a map.");
      }
      else if (marker == 'e') {
        addMessage(interface, "The note reads: I ran into a another group of explorers today.");
        addMessage(interface, "They told me that some of the stone walls are more brittle than others");
        addMessage(interface, "and could be broken. No help to me. I can't tell any of these apart.");
      }
      else if (marker == 'p'){
        addMessage(interface, "It appears to be mining tool. You pick it up");
        // TODO a rather janky way to do this, should mk
        interface->inventory[1] = "Feeble pickaxe";
        setPositionInMap(map, player->y, player->x, MAP_OPEN_SPACE);
      }
      else
        addMessage(interface, "You are surrounded by rock and darkness. Nothing interesting here.");
      break;
    case INTERACTION_TYPE_TALK:
      if (marker == 'd'){
        addMessage(interface, "You ask the skeleton for directions, ");
        addMessage(interface, "but are unsurprised when it does not respond.");
        addMessage(interface, "You ask yourself why you tried to ask a corpse for directions.");
      }
      else if (marker == 'p') {
        addMessage(interface, "You try talking to the object, but hear only silence.");
        addMessage(interface, "It appears to be dead, inanimate, or at least not responsive");
      }
      else
        addMessage(interface, "You start speaking but trail off. No one is around to hear.");
    default:
      break;
  }
}

void useItem(struct Player *player, struct Map *map, struct Interface *interface, int itemIndex){
  clearMessage(interface);
  char * item = interface->inventory[itemIndex];
  if (strcmp(item, "Tuna sandwhich") == 0){
    addMessage(interface, "You eat the sandwhich.");
    addMessage(interface, "It is delicious and nutritous.");
    interface->inventory[itemIndex] = "";
  }
  else if (strcmp(item, "Feeble pickaxe") == 0){
    int forwardX = round(player-> x + cos(player->direction) * PLAYER_FORWARDS);
    int forwardY = round(player-> y + sin(player->direction) * PLAYER_FORWARDS);
    char position = getPositionInMap(map, forwardY, forwardX);
    switch(position) {
      case MAP_OPEN_SPACE:
        addMessage(interface, "You swing the pickaxe. It strikes nothing.");
        break;
      case 'F':
        addMessage(interface, "You swing the pickaxe against the rock. It crumbles apart");
        setPositionInMap(map, forwardY, forwardX, MAP_OPEN_SPACE);
        break;
      default:
        // TODO, we don't want this to hit markers
        // TODO make it break
        addMessage(interface, "You swing the pickaxe, and it breaks");
        addMessage(interface, "Well that is a bummer");
    }
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

void walkAnimation(struct Player *player, struct Map *map, short direction){
  double moveSpeed = .1;
  double distance = 1.0;
  double finalX = player-> x + cos(player->direction) * distance * direction;
  double finalY = player-> y + sin(player->direction) * distance * direction;
  char nextPosition = getPositionInMap(map, (int) round(finalY), (int) round(finalX));
  if ( !(nextPosition == MAP_OPEN_SPACE || nextPosition > MAP_MARKER_MIN))
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

  char * mapcp = (char *) malloc(sizeof(char) * (strlen(&map1) + 1));
  strcpy(mapcp, map1);
  struct Map map = {20, mapcp};
  struct Player player = {1, 1, 0, 0, 0.66};
  struct Interface interface = {
    textWindow,
    {NULL, NULL, NULL},
    {"Tuna sandwhich", ""}
  };
  update(&player, &map, WIDTH, HEIGHT);
  updateInterface(&interface);

  while (1){
    char input = getch();
    switch(input) {
      case 'w':
        walkAnimation(&player, &map, PLAYER_FORWARDS);
        checkInteraction(&player, &map, &interface, INTERACTION_TYPE_WALK);
        break;
      case 's':
        walkAnimation(&player, &map, PLAYER_BACKWARDS);
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
      case '1':
        useItem(&player, &map, &interface, 0);
        break;
      case '2':
        useItem(&player, &map, &interface, 1);
        break;
      default:
        break;
    }
    update(&player, &map, WIDTH, HEIGHT);
    updateInterface(&interface);
  }

  endwin();
}
