
#ifndef TEXTBOX_H_   /* Include guard */
#define TEXTBOX_H_

#include <ncurses.h>

#define INTERFACE_MESSAGES_SIZE 3
#define INTERFACE_INVENTORY_SIZE 2

struct Interface {
  WINDOW * window;
  char * messages[INTERFACE_MESSAGES_SIZE];
  char * inventory[INTERFACE_INVENTORY_SIZE];
};

void updateInterface(struct Interface *textbox);
void addMessage(struct Interface *interface, char *message);
void clearMessage(struct Interface *interface);
#endif
