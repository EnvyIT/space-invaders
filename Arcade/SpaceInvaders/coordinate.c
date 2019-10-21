#include <stdlib.h>

#include "coordinate.h"
#include "utils.h"


Coordinate* create_coordinate(float x, float y) {
  Coordinate* newCoordinate = assert_not_null((Coordinate*)malloc(sizeof(Coordinate)), "Coordinate");
  newCoordinate->x = x;
  newCoordinate->y = y;
  return newCoordinate;
}

void destroy_coordinate(Coordinate** coordinate) {
  free(coordinate);
  coordinate = NULL;
}
