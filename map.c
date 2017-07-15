#include "map.h"

char getPositionInMap(struct Map *map, int y, int x) {
  if (y > map->size || y < 0 || x < 0 || x > map->size) {
    // Everything outside the map is open
    return MAP_OPEN_SPACE;
  }
  return map->map[map->size * y + x];
}
