#ifndef galaxy_h
#define galaxy_h
#endif // !

#include <allegro5/bitmap.h>

typedef struct Galaxy{
  ALLEGRO_BITMAP* image;
} Galaxy;


Galaxy* create_galaxy(void);
void destroy_galaxy(Galaxy** galaxy);
