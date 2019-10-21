#include "resources.h"
#include "utils.h"

#define FONT_SIZE 38
#define FONT_FILE ".\\Fonts\\space_invaders.ttf"
#define START_SCREEN_FILE ".\\Assets\\start_screen.bmp"

static const char* mainAssets[4] = {
  ".\\Assets\\alien1A.bmp",
  ".\\Assets\\alien2A.bmp",
  ".\\Assets\\alien2A.bmp",
  ".\\Assets\\alien3A.bmp"
};

static const char* offAssets[4] = {
  ".\\Assets\\alien1B.bmp",
  ".\\Assets\\alien2B.bmp",
  ".\\Assets\\alien2B.bmp",
  ".\\Assets\\alien3B.bmp"
};

static void system_init(void) {
  exitGame = false;
  init_or_exit(al_init(), "allegro");
  init_or_exit(al_install_keyboard(), "keyboard");
  init_or_exit(al_init_image_addon(), "image-addon");
  init_or_exit(al_init_ttf_addon(), "ttf add-on");
  init_or_exit(al_init_primitives_addon(), "primitives add-on");

  al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
  al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
  al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
  al_set_config_value(al_get_system_config(), "trace", "level", "debug");
  al_register_assert_handler(assert_handler);
}


static void init_assets(void) {
  timer = assert_not_null(al_create_timer(REFRESH_RATE), "timer");
  queue = assert_not_null(al_create_event_queue(), "event queue");
  display = assert_not_null(al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT), "display");
  font = assert_not_null(al_load_ttf_font(FONT_FILE, FONT_SIZE, 0), "space invaders font");
}

static void destroy_assets(void) {
  al_destroy_font(font);
  al_destroy_display(display);
  al_destroy_event_queue(queue);
  al_destroy_timer(timer);
}

static void register_events(void) {
  al_register_event_source(queue, al_get_timer_event_source(timer));
  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_display_event_source(display));
}

static void unregister_events(void) {
  al_unregister_event_source(queue, al_get_display_event_source(display));
  al_unregister_event_source(queue, al_get_keyboard_event_source());
  al_unregister_event_source(queue, al_get_timer_event_source(timer));
}

void init_resources(void) {
  system_init();
  init_assets();
  register_events();
}

void free_resources(void) {
  exitGame = true;
  unregister_events();
  destroy_assets();
}

void set_font_size(int size) {
  al_destroy_font(font);
  font = assert_not_null(al_load_ttf_font(FONT_FILE, size, 0),"reloading font size");
}

ALLEGRO_BITMAP* get_start_screen(void) {
  return assert_not_null(al_load_bitmap(START_SCREEN_FILE), "loading start screen");
}

static bool isIndexInRange(int index) {
  return index >= 0 && index < 4;
}

const char* get_main_asset(int index) {
  if (isIndexInRange(index)) {
    return mainAssets[index];
  }
  return NULL;
}

const char* get_off_asset(int index) {
  if (isIndexInRange(index)) {
    return offAssets[index];
  }
  return NULL;
}

AlienType get_alien_type(int index) {
  if (isIndexInRange(index)) {
    switch (index) {
    case 0 : return TypeC;
    case 1 : // fallthrough
    case 2 : return TypeB;
    default: return TypeA;
    }
  }
  return NoType;
}

void timeout(double seconds) {
  al_rest(seconds);
}