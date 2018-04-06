#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "engine.h"

#define ROWS 40
#define COLS 100
#define MIN_SWITCH 'a'
#define MAX_SWITCH 'j'

const int mapSize = 20;
const char map1[] = ""\
"- ------------------"\
"*                 *"\
"*   &XX&XX XX XX-  *"\
"*   &EEEEE EE EE-  *"\
"*   &E   E E       *"\
"*   &E   E EE  E-  *"\
"*   &E   E E   E-  *"\
"*   &E   E E   E-  *"\
"*   &E   E E   E-  *"\
"*   &E   E E   E-  *"\
"*   &&   E E   E-  *"\
"*   &    E E   E-  *"\
"*   &    EaE   E-  *"
"*   & &XXEEEXEEEEEX*"\
"*     XXXXXXXXXXX  *"\
"*                  *"\
"*                  *"\
"*                  *"\
"*                  *"\
"*&&&&&&&&&&&&&&&&&&&";

ENGINE * engine;
char *flag;

char * readFlag() {
  FILE * pFile;
  long lSize;
  size_t result;

  pFile = fopen("./flag", "rb" );
  fseek(pFile , 0 , SEEK_END);
  lSize = ftell(pFile);
  char * f = (char *) malloc(sizeof(char) * lSize + 1);
  rewind (pFile);
  result = fread(f, 1, lSize, pFile);
  f[lSize] = '\0'; 
  fclose (pFile);
  return f;
}

void checkInteraction(ENGINE *engine, INTERACTION interactionType) {
  clearMessage(engine->interface);
  char marker = getPositionInMap(engine->map, engine->player->y, engine->player->x);
  switch (interactionType){
    case INTERACTION_TYPE_WALK:
      if (marker == 's') {
          addMessage(engine->interface, "Suddenly giant spikes emerge from the floor");
          addMessage(engine->interface, "It looks like this is the end for you...");
          engine->gameState = STATE_DEAD;
      }
      else if (marker == 'a') {
         
      }
      else {
        char * pos = (char *) malloc(30 * sizeof(char));
        sprintf(pos, "Position: 0x%x", &engine->map->map[(int)engine->player->y * engine->map->size + (int)engine->player->x]);
        addMessage(engine->interface, pos);
        char * map = (char *) malloc(30 * sizeof(char));
        sprintf(map, "Map start: 0x%x", engine->map->map);
        addMessage(engine->interface, map);
        char * position = (char *) malloc(30 * sizeof(char));
        sprintf(position, "Flag location: 0x%x", flag);
        addMessage(engine->interface, position);
      }
      break;
    case INTERACTION_TYPE_LOOK:
        addMessage(engine->interface, "You are surrounded by rock and darkness. Nothing interesting here.");
      break;
    case INTERACTION_TYPE_TALK:
        addMessage(engine->interface, "Its so empty here. You wonder if anyone will hear your voice again.");
      break;
    default:
      break;
  }
}

void useItem(ENGINE *engine, char * item, int itemIndex) {
}

void clean(){
    free(flag);
    shutdown(engine);
}

int main(){
    flag = readFlag();
    engine = createEngine(ROWS, COLS, &map1, mapSize, 1, 1, 6, &checkInteraction, &useItem); 
    atexit(clean);
    signal(SIGTERM, exit);
    gameLoop(engine);
}
