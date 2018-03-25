#ifndef ENGINE_H
#define ENGINE_H

typedef unsigned short INTERACTION;

#define INTERACTION_TYPE_WALK 0
#define INTERACTION_TYPE_LOOK 1
#define INTERACTION_TYPE_TALK 2
#define INTERACTION_TYPE_ITEM 3

#include <ncurses.h>
#include <time.h>

#include "player.h"
#include "map.h"
#include "raycaster.h"
#include "interface.h"

extern const struct timespec FRAME_DELAY;

typedef struct Engine {
  int rows;
  int cols;
  WINDOW *mainWindow;
  struct Player *player;
  struct Map *map;
  struct Interface *interface;
  void (*interactionFn)(struct Engine *, INTERACTION);
} ENGINE;

typedef void (*INTERACTION_FUNCTION)(ENGINE *, INTERACTION);

// create a new game engine
ENGINE * createEngine(int rows, int cols,
  const char *mapStr,
  int mapSize, int playerStartR, int playerStartC,
  int interfaceHeight,
  INTERACTION_FUNCTION interactionFn
);

// run the game loop
void gameLoop(ENGINE *engine);

// shutdown the game
void shutdown(ENGINE *engine);
#endif
