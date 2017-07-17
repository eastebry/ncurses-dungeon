
#ifndef MAP_H_/* Include guard */
#define MAP_H_

#define MAP_OPEN_SPACE ' '

struct Map {
  int size;
  char *map;
};

char getPositionInMap(struct Map *map, int y, int x);
void setPositionInMap(struct Map *map, int y, int x, char newChar);

#endif
