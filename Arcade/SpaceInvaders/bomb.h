#ifndef bomb_h
#define bomb_h
#endif // !bomb_h

#include <allegro5/bitmap.h>

#include "coordinate.h"

typedef struct Bomb {
  float velocity;
  Coordinate* coordinate;
  ALLEGRO_BITMAP* image;
} Bomb;

Bomb* create_bomb(float x, float y);
void destroy_bomb(Bomb** bomb);