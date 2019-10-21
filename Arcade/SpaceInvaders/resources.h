#ifndef resources_h
#define resources_h
#endif // !

#include <stdbool.h>
#include <allegro5/allegro5.h>
#include <allegro5/timer.h>
#include <allegro5/altime.h>
#include "allegro5/allegro_image.h"
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#include "spaceship.h"
#include "galaxy.h"
#include "alien.h"
#include "laserbeam.h"
#include "bomb.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 680
#define REFRESH_RATE (1.0 / 35)

bool exitGame;

ALLEGRO_TIMER* timer;
ALLEGRO_EVENT_QUEUE* queue;
ALLEGRO_DISPLAY* display;
ALLEGRO_FONT* font;

void init_resources(void);
void free_resources(void);

void set_font_size(int size);
ALLEGRO_BITMAP* get_start_screen(void);

const char* get_main_asset(int index);
const char* get_off_asset(int index);
AlienType get_alien_type(int index);

void timeout(double seconds);
