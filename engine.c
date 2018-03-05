#include <ncurses.h>
#include "engine.h"
#include "player.h"
#include "map.h"
#include "raycaster.h"
/*
const struct timespec DELAY = {0, 90000000L};

ENGINE * createEngine(int rows, int cols, const char *mapStr,
  int mapSize, int playerStartR, int playerStartC,
  void *interactionFn(ENGINE *, INTERACTION)) {
  //resizeterm(WIDTH, HEIGHT);
  initscr();
  start_color();
  initColors();
  cbreak();
  noecho();
  //nodelay(stdscr, TRUE);
  curs_set(FALSE);

  // TODO what is up with these params being different?
  WINDOW * mainWindow = newwin(HEIGHT, WIDTH, 0, 0);
  WINDOW * textWindow = newwin(6, WIDTH, HEIGHT, 0);

  // TODO is there a way to malloc and assign simultaneously
  struct Player * player = (struct Player *) malloc(sizeof(struct Player));
  player->x = playerStartR;
  player->y = playerStartC;
  player->direction = 0;
  player->cameraPlaneX = 0;
  player->cameraPlaneY = .66; // Seems to be about the optimal view

  struct Interface * interface = (struct Interface *) calloc(sizeof(struct Interface));
  interface->window = textWindow;
  interface->messages = (char **) calloc(sizeof(char *) * INTERFACE_MESSAGES_SIZE);
  interface->inventory = (char **) calloc(sizeof(char *) * INTERFACE_INVENTORY_SIZE);

  struct Map *map = (Map *) malloc(sizeof(struct Map));
  map->map = (char *) calloc(sizeof(char) * (strlen(mapStr) + 1));
  strcpy(mapStr, map->map);
  map->size = mapSize;

  ENGINE * engine = (ENGINE * ) malloc(sizeof(ENGINE));
  engine->rows = rows;
  engine->cols = cols;
  engine->window = mainWindow;
  engine->player = player;
  engine->map = map;
  engine->interface = interface;

  return engine;
}

void renderFrame(ENGINE *engine){
  werase(engine->mainWindow);
  raycast(engine->player, engine->map, engine->mainWindow, w, h);
  //drawMiniMap(player, map);
  wrefresh(engine->mainWindow);
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

void checkInteraction(ENGINE * engine, INTERACTION interactionType){
  // call into the function pointers provided by the client

}

void gameLoop(ENGINE *engine){
  updateInterface(engine->interface);
  renderFrame(engine);

  while (1){
    char input = getch();
    switch(input) {
      case 'w':
        walkAnimation(&player, &map, PLAYER_FORWARDS);
        checkInteraction(&player, &map, &interface, INTERACTION_TYPE_WALK);
        break;
      case 's':
        walkAnimation(&player, &map, PLAYER_BACKWARDS);
        engine->interactionFn(engine, INTERACTION_TYPE_WALK);
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
}

void shutdown(ENGINE *engine){
  // TODO call delwin()
  // TODO free everything here
  endwin();
}

*/
