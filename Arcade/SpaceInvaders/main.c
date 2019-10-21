#include <stdbool.h>

#include "resources.h"
#include "keycodes.h"
#include "game.h"

static ALLEGRO_EVENT event;
static ALLEGRO_BITMAP* startScreen = NULL;

void main_event_loop() {
  al_start_timer(timer);
  while (!exitGame) {
    al_wait_for_event(queue, &event);
    switch(event.type) {
      case ALLEGRO_EVENT_TIMER: update_game(); break;
      case ALLEGRO_EVENT_KEY_CHAR: key_char(event.keyboard); break;
      case ALLEGRO_EVENT_DISPLAY_CLOSE: exitGame = true; break;
    }
  }
}
static void show_start_screen(void) {
  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_draw_bitmap(startScreen, 35, 0, 0);
  al_flip_display();
}

static void blink_press_start() {
  show_start_screen();
  al_draw_text(font, al_map_rgb(255, 255, 255), WINDOW_WIDTH / 3.0F, WINDOW_HEIGHT / 1.33F, 0, "PRESS < ENTER > to START");
  al_flip_display();
}

int main() {
  setup_game();
  startScreen = get_start_screen();
  do {
    blink_press_start();
    al_wait_for_event(queue, &event);
  } while(event.keyboard.keycode != ENTER);
  start_game();
  main_event_loop();
  return 0;
}