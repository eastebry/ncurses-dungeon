#include <ncurses.h>
#include "interface.h"

void updateInterface(struct Interface *interface){
  WINDOW *window = interface->window;
  wclear(window);
  wattron(window, A_BOLD);
  box(window, 0, 0);
  wattroff(window, A_BOLD);

  for (int i = 0; i < INTERFACE_MESSAGES_SIZE; i++) {
    if (interface->messages[i] != NULL)
      mvwprintw(window, 1 + i, 2, interface->messages[i]);
  }
  mvwprintw(window, 4, 2, ">> ");

  wattron(window, A_BOLD);
  mvwprintw(window, 1, 80, "Commands");
  wattroff(window, A_BOLD);
  mvwprintw(window, 2, 80, "(L)ook   (W/A/S/D) Move");
  mvwprintw(window, 3, 80, "(T)alk   (1/2/3/4) Use item");

  wattron(window, A_BOLD);
  mvwprintw(window, 1, 115, "Inventory");
  wattroff(window, A_BOLD);
  mvwprintw(window, 2, 115, "1) ");
  mvwprintw(window, 2, 118, interface->inventory[0]);
  //mvwprintw(window, 3, 115, "2) ");
  //mvwprintw(window, 3, 118, interface->inventory[1]);
  wrefresh(window);
}

void addMessage(struct Interface *interface, char *message) {
  // shift everything down
  for (int i = 0; i < INTERFACE_MESSAGES_SIZE; i++){
    if (i == INTERFACE_MESSAGES_SIZE - 1) {
      interface->messages[i] = message;
      return;
    }
    interface->messages[i] = interface->messages[i + 1];
  }
  updateInterface(interface);
}

void clearMessage(struct Interface *interface){
  for (int i = 0; i < INTERFACE_MESSAGES_SIZE; i++){
    interface->messages[i] = NULL;
  }
}
