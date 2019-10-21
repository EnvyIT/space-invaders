#include <stdlib.h>
#include <allegro5/allegro5.h>

#include "laserbeam.h"
#include "utils.h"

#define LASER_VELOCITY 10.5F

LaserBeam* create_laser_beam(float x, float y) {
  LaserBeam* newLaserBeam = assert_not_null((LaserBeam*)malloc(sizeof(LaserBeam)), "LaserBeam");
  newLaserBeam->image = assert_not_null(al_load_bitmap(".\\Assets\\laser_beam.bmp"), "LaserBeam bitmap");
  assert_not_null(newLaserBeam->image, "LaserBeam bitmap");
  newLaserBeam->coordinate = create_coordinate(x, y);
  assert_not_null(newLaserBeam->coordinate, "LaserBeam coordinates");
  newLaserBeam->velocity = LASER_VELOCITY;
  return newLaserBeam;
}

void destroy_laser_beam(LaserBeam** laserBeam) {
  al_destroy_bitmap((*laserBeam)->image);
  free((*laserBeam)->coordinate);
  free(*laserBeam);
}
