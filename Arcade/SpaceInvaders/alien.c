#include <stdlib.h>
#include <allegro5/allegro.h>

#include "alien.h"
#include "utils.h"


#define ALIEN_VELOCITY 0.85F

Alien* create_alien(float x, float y, const char* alienImg, const char* alienMoveImg, AlienType alienType) {
  Alien* newAlien = assert_not_null((Alien*)malloc(sizeof(Alien)), "Alien");
  newAlien->image = assert_not_null(al_load_bitmap(alienImg), "Alien bitmap");
  assert_not_null(newAlien->image, "Alien bitmap");
  newAlien->destroyed_image = assert_not_null(al_load_bitmap(".\\Assets\\explode.bmp"), "Alien destroyed bitmap");
  assert_not_null(newAlien->destroyed_image, "Alien destroyed bitmap");
  newAlien->image_move = assert_not_null(al_load_bitmap(alienMoveImg), "Alien move bitmap");
  assert_not_null(newAlien->image_move, "Alien move bitmap");
  newAlien->coordinate = create_coordinate(x, y);
  assert_not_null(newAlien->coordinate, "Alien coordinates");
  newAlien->velocity = ALIEN_VELOCITY;
  newAlien->is_alive = true;
  newAlien->points = (int)alienType;
  return newAlien;
}

void destroy_alien(Alien** alien) {
  al_destroy_bitmap((*alien)->image);
  al_destroy_bitmap((*alien)->image_move);
  al_destroy_bitmap((*alien)->destroyed_image);
  free((*alien)->coordinate);
  free(alien);
  alien = NULL;
}
