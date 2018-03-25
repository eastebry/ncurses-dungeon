#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "engine.h"

#define ROWS 40
#define COLS 100

const int mapSize = 20;
const char map1[] = ""\
"********************"\
"X  ab c dd&     *  X"\
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

void checkInteraction(ENGINE *engine, INTERACTION interactionType){
  clearMessage(engine->interface);
  char marker = getPositionInMap(engine->map, engine->player->y, engine->player->x);
  switch (interactionType){
    case INTERACTION_TYPE_WALK:
      if (marker == 'a'){
        addMessage(engine->interface, "Immediately after entering, you hear a thunderous boom");
        addMessage(engine->interface, "The wall ceiling behind you collapse into a pile of rubble");
        addMessage(engine->interface, "You barely avoid the falling rock, but you are now trapped");
      }
      else if (marker == 'b')
        addMessage(engine->interface, "The air here is thick and damp.");
      else if (marker == 'c')
        addMessage(engine->interface, "You feel a light breeze. This dungeon is large and cavernous.");
      else if (marker == 'd'){
        addMessage(engine->interface, "There is a skeleton lying on the ground. It has clearly been here for while.");
        addMessage(engine->interface, "The bones of its fingers are curled around a piece of parchment.");
        addMessage(engine->interface, "Press (L) to read the note.");
      }
      else if (marker == 'e'){
        addMessage(engine->interface, "There is a another skeleton on the ground.");
        addMessage(engine->interface, "There some paper lying on the ground next to it.");
      }
      else if (marker == 'p') {
        addMessage(engine->interface, "There's an object lying on the ground.");
      }
      break;
    case INTERACTION_TYPE_LOOK:
      if (marker == 'd') {
        addMessage(engine->interface, "The note reads: I've been trapped in this maze for weeks.");
        addMessage(engine->interface, "This dungeon is a labyrenth. I've expored every passageway ten times,");
        addMessage(engine->interface, "but still can't find where I entered. I wish I had a map.");
      }
      else if (marker == 'e') {
        addMessage(engine->interface, "The note reads: I ran into a another group of explorers today.");
        addMessage(engine->interface, "They told me that some of the stone walls are more brittle than others");
        addMessage(engine->interface, "and could be broken. No help to me. I can't tell any of these apart.");
      }
      else if (marker == 'p'){
        addMessage(engine->interface, "It appears to be mining tool. You pick it up");
        addItem(engine->interface, "Feeble pickaxe");
        setPositionInMap(engine->map, engine->player->y, engine->player->x, MAP_OPEN_SPACE);
      }
      else
        addMessage(engine->interface, "You are surrounded by rock and darkness. Nothing interesting here.");
      break;
    case INTERACTION_TYPE_TALK:
      if (marker == 'd'){
        addMessage(engine->interface, "You ask the skeleton for directions, ");
        addMessage(engine->interface, "but are unsurprised when it does not respond.");
        addMessage(engine->interface, "You ask yourself why you tried to ask a corpse for directions.");
      }
      else if (marker == 'p') {
        addMessage(engine->interface, "You try talking to the object, but hear only silence.");
        addMessage(engine->interface, "It appears to be dead, inanimate, or at least not responsive");
      }
      else
        addMessage(engine->interface, "You start speaking but trail off. No one is around to hear.");
    default:
      break;
  }
}

void useItem(ENGINE *engine, int itemIndex) {
  clearMessage(engine->interface);
  char * item = engine->interface->inventory[itemIndex];
  if (strcmp(item, "Tuna sandwhich") == 0){
    addMessage(engine->interface, "You eat the sandwhich.");
    addMessage(engine->interface, "It is delicious and nutritous.");
    engine->interface->inventory[itemIndex] = "";
  }
  else if (strcmp(item, "Feeble pickaxe") == 0) {
    int forwardX = round(engine->player-> x + cos(engine->player->direction) * PLAYER_FORWARDS);
    int forwardY = round(engine->player-> y + sin(engine->player->direction) * PLAYER_FORWARDS);
    char position = getPositionInMap(engine->map, forwardY, forwardX);
    switch(position) {
      case MAP_OPEN_SPACE:
        addMessage(engine->interface, "You swing the pickaxe. It strikes nothing.");
        break;
      case 'F':
        addMessage(engine->interface, "You swing the pickaxe against the rock. It crumbles apart");
        setPositionInMap(engine->map, forwardY, forwardX, MAP_OPEN_SPACE);
        break;
      default:
        // TODO, we don't want this to hit markers
        // TODO make it break
        addMessage(engine->interface, "You swing the pickaxe, and it breaks");
        addMessage(engine->interface, "Well that is a bummer");
        break;
    }
  }
}

int main(){
    ENGINE *engine = createEngine(ROWS, COLS, &map1, mapSize, 1, 1, 6, &checkInteraction); 
    gameLoop(engine);
}
