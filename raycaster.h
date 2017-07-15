
#ifndef RAYCAST_H_   /* Include guard */
#define RAYCAST_H_

#include "player.h"
#include "map.h"

// Color definitions
#define WALL_RED 1
#define WALL_GREEN 2
#define WALL_YELLOW 3
#define WALL_CYAN 4
#define FLOOR 5
#define BLACK 6

// shorthand for color pairs
#define ATTR_ALL_BLACK COLOR_PAIR(BLACK)
#define ATTR_WALL_RED COLOR_PAIR(WALL_RED)
#define ATTR_WALL_GREEN COLOR_PAIR(WALL_GREEN)
#define ATTR_WALL_YELLOW COLOR_PAIR(WALL_YELLOW)
#define ATTR_WALL_CYAN COLOR_PAIR(WALL_CYAN)
#define ATTR_FLOOR COLOR_PAIR(FLOOR)

void drawColumn(int column, double colHeight, int screenHeight, char character);
void raycast (struct Player *player, struct Map *map, int width, int height);

#endif
