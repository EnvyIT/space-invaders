#pragma once

typedef struct {
  float x;
  float y;
} Coordinate;

Coordinate* create_coordinate(float x, float y);
void destroy_coordinate(Coordinate** coordinate);