#ifndef alien_h
#define alien_h
#endif // !

#include <allegro5/bitmap.h>

#include "coordinate.h"

#define ALIEN_WIDTH 24
#define ALIEN_HEIGHT 16

typedef struct Alien {
  bool is_alive;
  float velocity;
  int points;
  Coordinate* coordinate;
  ALLEGRO_BITMAP* image;
  ALLEGRO_BITMAP* image_move;
  ALLEGRO_BITMAP* destroyed_image;
} Alien;

typedef enum AlienType {
  NoType = 0,
  TypeA = 10,
  TypeB = 20,
  TypeC = 35
} AlienType;

Alien* create_alien(float x , float y, const char* alienImg, const char* alienMoveImg, AlienType alienType);
void destroy_alien(Alien** alien);