#ifndef ENGINE_H
#define ENGINE_H

typedef unsigned short INTERACTION;

#define INTERACTION_TYPE_WALK 0
#define INTERACTION_TYPE_LOOK 1
#define INTERACTION_TYPE_TALK 2

#include <ncurses.h>

#include "player.h"
#include "map.h"
#include "raycaster.h"
#include "interface.h"
/*
typedef struct Engine {
  int rows;
  int cols;
  struct WINDOW *mainWindow;
  struct Player *player;
  struct Map *map;
  struct Interface *interface;
  void (*interactionFn)(ENGINE *, INTERACTION);
} ENGINE;

// create a new game engine
ENGINE * createEngine(int rows, int cols, const char *mapStr);

// run the game loop
void gameLoop(ENGINE *engine);

// shutdown the game
void shutdown(ENGINE *engine);
*/
#endif
