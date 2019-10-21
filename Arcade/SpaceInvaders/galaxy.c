#include <stdlib.h>
#include <allegro5/allegro5.h>

#include "utils.h"
#include "galaxy.h"

Galaxy* create_galaxy() {
  Galaxy* newGalaxy = assert_not_null((Galaxy*)malloc(sizeof(Galaxy)), "Galaxy");
  newGalaxy->image = assert_not_null(al_load_bitmap(".\\Assets\\space.bmp"), "Galaxy bitmap");
  assert_not_null(newGalaxy->image, "Galaxy Bitmap");
  return newGalaxy;
}

void destroy_galaxy(Galaxy** galaxy) {
  al_destroy_bitmap((*galaxy)->image);
  free(galaxy);
  galaxy = NULL;
}
