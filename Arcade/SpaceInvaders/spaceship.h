#ifndef spaceship_h
#define spaceship_h
#endif // !

#include <allegro5/bitmap.h>

#include "coordinate.h"

#define SPACE_SHIP_WIDTH 26
#define SPACE_SHIP_HEIGHT 16

typedef struct SpaceShip{
  int lifes;
  int score;
  float velocity;
  Coordinate* coordinate;
  ALLEGRO_BITMAP* image;
  ALLEGRO_BITMAP* destroyed_image;
} SpaceShip;

SpaceShip* create_space_ship(float x, float y);
void destroy_space_ship(SpaceShip** spaceShip);