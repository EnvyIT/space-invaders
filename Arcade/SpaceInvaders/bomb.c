#include <stdlib.h>
#include <allegro5/allegro5.h>

#include "bomb.h"
#include "utils.h"


#define BOMB_VELOCITY 9.5F

Bomb* create_bomb(float x, float y) {
  Bomb* newBomb = assert_not_null((Bomb*)malloc(sizeof(Bomb)), "Bomb");
  newBomb->image = assert_not_null(al_load_bitmap(".\\Assets\\bomb.bmp"), "Bomb bitmap");
  assert_not_null(newBomb->image, "Bomb bitmap");
  newBomb->coordinate = create_coordinate(x, y);
  assert_not_null(newBomb->coordinate, "Bomb coordinates");
  newBomb->velocity = BOMB_VELOCITY;
  return newBomb;
}

void destroy_bomb(Bomb** bomb) {
  al_destroy_bitmap((*bomb)->image);
  free((*bomb)->coordinate);
  free(*bomb);
}
