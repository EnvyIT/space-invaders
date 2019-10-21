#ifndef laserbeam_h
#define laserbeam_h
#endif // !

#include <allegro5/bitmap.h>

#include "coordinate.h"

typedef struct LaserBeam {
  float velocity;
  Coordinate* coordinate;
  ALLEGRO_BITMAP* image;
} LaserBeam;

LaserBeam* create_laser_beam(float x, float y);
void destroy_laser_beam(LaserBeam** spaceShip);
