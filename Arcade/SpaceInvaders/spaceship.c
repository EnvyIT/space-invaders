#include <stdlib.h>
#include <allegro5/allegro5.h>

#include "spaceship.h"
#include "utils.h"

#define SHIP_VELOCITY 7.7F
#define LIFES 3

SpaceShip* create_space_ship(float x, float y) {
  SpaceShip* newSpaceShip = assert_not_null((SpaceShip*)malloc(sizeof(SpaceShip)), "SpaceShip");
  newSpaceShip->image = assert_not_null(al_load_bitmap(".\\Assets\\spaceship.bmp"), "SpaceShip Bitmap");
  assert_not_null(newSpaceShip->image, "SpaceShip Bitmap");
  newSpaceShip->destroyed_image = assert_not_null(al_load_bitmap(".\\Assets\\spaceship_explode.bmp"), "SpaceShip explode Bitmap");
  assert_not_null(newSpaceShip->image, "SpaceShip explode Bitmap");
  newSpaceShip->coordinate = create_coordinate(x, y);
  assert_not_null(newSpaceShip->coordinate, "SpaceShip coordinates");
  newSpaceShip->velocity = SHIP_VELOCITY;
  newSpaceShip->lifes = LIFES;
  newSpaceShip->score = 0;
  return newSpaceShip;
}

void destroy_space_ship(SpaceShip** spaceShip) {
  al_destroy_bitmap((*spaceShip)->image);
  free((*spaceShip)->coordinate);
  free(spaceShip);
  spaceShip = NULL;
}
