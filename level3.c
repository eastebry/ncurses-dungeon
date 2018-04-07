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
#define MAX_STEPS 1024 
#define NULL_STEP 255

const int mapSize = 20;
const char map1[] = ""\
"- ------------------"\
"*                  *"\
"*   &XX&XX XX XX-  *"\
"*   &EEEEE EE EE-  *"\
"*   &E   E EE  E-  *"\
"*   &E   E EE  E-  *"\
"*   &E   E EE  E-  *"\
"*   &E   E EE  E-  *"\
"*   &E   E EE  E-  *"\
"*   &E   E EE  E-  *"\
"*   &&   EaEE  E-  *"\
"*   &    E EE  E-  *"\
"*   &    E EE  E-XX*"
"*   & &XXEEEXEEEEEX*"\
"*     XXXXXXXXXXXXX*"\
"*                  *"\
"*                  *"\
"*                  *"\
"*                  *"\
"*&&&&&&&&&&&&&&&&&&&";

ENGINE * engine;
char *flag;
unsigned char steps[MAX_STEPS];
unsigned int stepIndex = -1;


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

bool hasHourglass(ENGINE *engine){
  if ( (engine->interface->inventory[0] != NULL && strcmp(engine->interface->inventory[0], "Sands of time") == 0) || 
      (engine->interface->inventory[1] != NULL && strcmp(engine->interface->inventory[1], "Sands of time") == 0)) {
    return true;
  }
  return false;
}

void useHourglass(ENGINE *engine){
  switch (steps[stepIndex]) {
  case INTERACTION_TYPE_WALK_FORWARD:
    walkAnimation(engine, PLAYER_BACKWARDS);
    break;
  case INTERACTION_TYPE_WALK_BACK:
    walkAnimation(engine, PLAYER_FORWARDS);
    break;
  case INTERACTION_TYPE_TURN_RIGHT:
    rotationAnimation(engine, M_PI/2.0, PLAYER_COUNTER_CLOCKWISE);
    break;
  case INTERACTION_TYPE_TURN_LEFT:
    rotationAnimation(engine, M_PI/2.0, PLAYER_CLOCKWISE);
    break;
  case NULL_STEP:
    addMessage(engine->interface, "Nothing happened.");
    return;
  }
  steps[stepIndex] = NULL_STEP;
  stepIndex = (stepIndex - 1) % MAX_STEPS;
}

void checkInteraction(ENGINE *engine, INTERACTION interactionType) {
  clearMessage(engine->interface);
  if (interactionType == INTERACTION_TYPE_WALK_FORWARD ||
       interactionType == INTERACTION_TYPE_WALK_BACK ||
       interactionType == INTERACTION_TYPE_TURN_LEFT || 
       interactionType == INTERACTION_TYPE_TURN_RIGHT)
  {  
    stepIndex++;
    stepIndex %= MAX_STEPS;
    steps[stepIndex] = interactionType;
  }
    
  char marker = getPositionInMap(engine->map, engine->player->y, engine->player->x);
  switch (interactionType){
    case INTERACTION_TYPE_WALK_FORWARD:
    case INTERACTION_TYPE_WALK_BACK:
    if (marker == 's') {
      addMessage(engine->interface, "Suddenly giant spikes emerge from the floor");
      addMessage(engine->interface, "It looks like this is the end for you...");
      engine->gameState = STATE_DEAD;
      return;
    }
    else if (marker == 'a') {
      addMessage(engine->interface, "There is an item on a pedestal next to you");
      addMessage(engine->interface, "It appears to be some sort of hourglass");
      addMessage(engine->interface, "It looks ornate and ancient");
    }
    else {
      /*
      char * pos = (char *) malloc(30 * sizeof(char));
      sprintf(pos, "Position: 0x%x", &engine->map->map[(int)engine->player->y * engine->map->size + (int)engine->player->x]);
      addMessage(engine->interface, pos);
      char * map = (char *) malloc(30 * sizeof(char));
      sprintf(map, "Map start: 0x%x", engine->map->map);
      addMessage(engine->interface, map);
      char * position = (char *) malloc(30 * sizeof(char));
      sprintf(position, "Flag location: 0x%x", flag);
      addMessage(engine->interface, position);
      */
    }
    break;
    case INTERACTION_TYPE_LOOK:
    if (marker == 'a') {
        addMessage(engine->interface, "You pick up the hourglass.");
        addMessage(engine->interface, "Out of nowhere, you hear a thundering voice...");
        // TODO something related to this
        addMessage(engine->interface, "\"Cursed is he who holds the hourglass! Your footsteps are numbered!\"");
        addItem(engine->interface, "Sands of time");
        setPositionInMap(engine->map, engine->player->y, engine->player->x, MAP_OPEN_SPACE);
    }
    else {
        addMessage(engine->interface, "You are surrounded by rock and darkness. Nothing interesting here.");
    }
    break;
    case INTERACTION_TYPE_TALK:
    addMessage(engine->interface, "Its so empty here. You wonder if anyone will hear your voice again.");
    break;
    default:
    break;
  }
}

void useItem(ENGINE *engine, char * item, int itemIndex) {
  if (strcmp(item, "Sands of time") == 0) {
    clearMessage(engine->interface);
    addMessage(engine->interface, "You tipped the hourglass");
    useHourglass(engine);
  }
}

void clean(){
  free(flag);
  shutdown(engine);
}

int main(){
  flag = readFlag();
  memset(steps, NULL_STEP, sizeof(steps));
  engine = createEngine(ROWS, COLS, &map1, mapSize, 1, 1, 6, &checkInteraction, &useItem);
  atexit(clean);
  signal(SIGTERM, exit);
  gameLoop(engine);
}
