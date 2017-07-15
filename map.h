
#ifndef MAP_H_/* Include guard */
#define MAP_H_

#define MAP_OPEN_SPACE ' '

struct Map {
  int size;
  const char *map;
};

char getPositionInMap(struct Map *map, int y, int x);

#endif
