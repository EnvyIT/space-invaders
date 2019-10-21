// file: src/game.c
#include "game.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "resources.h"
#include "utils.h"
#include "keycodes.h"

#define ALIEN_ROWS 4
#define ALIEN_COLS 11
#define ALIENS_AMOUNT (ALIEN_ROWS * ALIEN_COLS)
#define ROW_SPACING 40
#define OFFSET 35
#define LEFT_EDGE 0
#define MAX_LASERS 200
#define MAX_BOMBS 70
#define BOMB_INTERVALL 60
#define SPACE_SHIP_OFFSET (SPACE_SHIP_HEIGHT * 3.25F)

static SpaceShip* spaceShip;
static Galaxy* galaxy;
static Alien** aliens;
static bool leftTurn = false;
static bool rightTurn = true;
static LaserBeam* laserBeams[MAX_LASERS]= { 0 };
static Bomb* bombs[MAX_BOMBS] = { 0 };
static int remainingAliens = 0;
static bool isPause = false;

/* forward declarations */
static void add_laser_beam(LaserBeam* laserBeam);
static void update_laser_beam(LaserBeam* laserBeam);
static void update_all_lasers(void);

static Alien* get_next_alien(void);
static void add_bomb(Bomb* bomb);
static void random_bomb(void);
static void update_random_bombs(void);
static void update_bomb(Bomb* bomb);
static void update_all_bombs(void);

static bool is_alien_hit(Alien* alien, LaserBeam* laserBeam);
static void update_alien_hits(void);
static void move_aliens(void);
static bool can_move_right(float xCoordinate, float offset);
static bool can_move_left(float xCoordinate, float offset);
static void move_right(void);
static void move_left(void);
static void move_down(void);
static void redraw_aliens_move_up(void);
static void redraw_aliens_move_down(void);
static void redraw_aliens(void);

static bool is_ship_hit(Bomb* bomb);
static void update_ship_hits(void);
static void update_ship(void);

static void init_aliens(void);
static void init_game_models(void);
static bool isGameWon(void);
static bool isGameOver(void);
static void draw_text(const char* text, int fontSize, float width, float height);
static void show_game_over(void);
static void show_win_screen(void);
static void show_dying_screen(void);
static void draw_background(void);
static void refresh_game(void);
static void show_score(void);
static bool has_invasion_begun(void);
static void end_game(void);
static void pause_game(void);
static void show_lifes(void);

void start_game(void);
void setup_game(void);
void update_game(void);
void key_char(ALLEGRO_KEYBOARD_EVENT event);
void shut_down_game(void);

/*public functions*/
void setup_game(void) {
  init_resources();
  set_font_size(18);
}

void start_game(void) {
  init_game_models();
}

void update_game(void) {
  if (!isGameWon() && !isGameOver()) {
    refresh_game();
  } else if (isGameOver()) {
    end_game();
  } else if (isGameWon()) {
    show_win_screen();
  }
}

static void end_game(void) {
  show_game_over();
}


static void pause_game(void) {
  isPause = !isPause;
  if (isPause) {
    al_stop_timer(timer);
    draw_text("GAME PAUSE ", 34, WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f);
    al_flip_display();
  } else {
    al_start_timer(timer);
  }
}

static void show_lifes(void) {
  al_draw_line(0.0F, WINDOW_HEIGHT - (SPACE_SHIP_HEIGHT*2.0F), WINDOW_WIDTH, WINDOW_HEIGHT - (SPACE_SHIP_HEIGHT * 2.0F), al_map_rgb(24, 219, 27), 2.0F);
  if (spaceShip != NULL) {
    int lifes = spaceShip->lifes;
    float x = 0.0F;
    float y = WINDOW_HEIGHT - SPACE_SHIP_HEIGHT;
    for (int i = 0; i < lifes; i++) {
      al_draw_bitmap(spaceShip->image, x + (SPACE_SHIP_WIDTH + OFFSET * i) , y, ALLEGRO_FLIP_HORIZONTAL);
    }
  }
}

void key_char(ALLEGRO_KEYBOARD_EVENT event) {
  if (isGameOver() || isGameWon()) {
    if (event.keycode) {
      exitGame = true;
    }
    return;
  } 
  if (event.keycode == ARROW_LEFT && can_move_left(spaceShip->coordinate->x, SPACE_SHIP_WIDTH/3)) {
    spaceShip->coordinate->x-= spaceShip->velocity;
  }
  if (event.keycode == ARROW_RIGHT && can_move_right(spaceShip->coordinate->x, SPACE_SHIP_WIDTH + SPACE_SHIP_WIDTH /3)) {
    spaceShip->coordinate->x+= spaceShip->velocity;
  }
  if (event.keycode == SPACE) {
    LaserBeam* laserBeam = create_laser_beam(spaceShip->coordinate->x + SPACE_SHIP_WIDTH/2, spaceShip->coordinate->y - SPACE_SHIP_HEIGHT/2);
    add_laser_beam(laserBeam);
  }
  if (event.keycode == ESCAPE) {
    pause_game();
  }
  update_ship();
}

void shut_down_game(void) {
  free_resources();
}
/*end public functions*/


/* util functions */
static void show_score(void) {
  draw_text("SCORE ", 20, 60, 20);
  if (spaceShip != NULL) {
    int score = spaceShip->score;
    int size = 20;
    char scoreNumber[20];
    sprintf_s(scoreNumber, size, " %06d ", score);
    draw_text(scoreNumber, 20, 150.0F, 20);
  }
}

static bool has_invasion_begun(void) {
  for (int i = 0; i < ALIEN_ROWS; i++) {
    for (int j = 0; j < ALIEN_COLS; j++) {
      if (aliens[i * ALIEN_COLS + j] != NULL && aliens[i * ALIEN_COLS + j]->coordinate->y >= (WINDOW_HEIGHT - SPACE_SHIP_OFFSET)) {
        return true;
      }
    }
  }
  return false;
}

static void init_aliens(void) {
  aliens = assert_not_null((Alien*)malloc(sizeof(Alien*) * ALIENS_AMOUNT), "Aliens array");
  remainingAliens = ALIENS_AMOUNT;
  int display_width = al_get_display_width(display);
  for (int i = 0; i < ALIEN_ROWS; i++) {
    for (int j = 0; j < ALIEN_COLS; j++) {
      aliens[i * ALIEN_COLS + j] = create_alien(j * (display_width / ALIEN_COLS), ROW_SPACING + (i * OFFSET), get_main_asset(i), get_off_asset(i), get_alien_type(i));
    }
  }
}

static void init_game_models(void) {
  srand(time(NULL));
  spaceShip = create_space_ship(WINDOW_WIDTH / 2.0F, WINDOW_HEIGHT - SPACE_SHIP_OFFSET);
  galaxy = create_galaxy();
  init_aliens();
}

static bool isGameWon(void) {
  return remainingAliens == 0;
}

static bool isGameOver(void) {
  return spaceShip->lifes == 0 || has_invasion_begun();
}

static void draw_text(const char* text, int fontSize, float width, float height) {
  set_font_size(fontSize);
  al_draw_text(font, al_map_rgb(255, 255, 255), width, height, ALLEGRO_FLIP_HORIZONTAL, text);
}

static void show_game_over(void) {
  al_clear_to_color(al_map_rgb(0, 0, 0));
  draw_text("GAME OVER", 34, WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f);
  al_flip_display();
}

static void show_win_screen(void) {
  al_clear_to_color(al_map_rgb(0, 0, 0));
  draw_text("The earth is save ... for now!", 34, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
  al_flip_display();
}

static void show_dying_screen(void) {
  if (spaceShip->lifes > 0) {
    spaceShip->lifes--;
  }
  al_draw_bitmap(spaceShip->destroyed_image, spaceShip->coordinate->x, spaceShip->coordinate->y, ALLEGRO_FLIP_HORIZONTAL);
  for (int i = 0; i < 1750; i++) {
    if (i > 1000) {
      al_draw_bitmap(spaceShip->image, spaceShip->coordinate->x, spaceShip->coordinate->y, ALLEGRO_FLIP_HORIZONTAL);
    }
    al_flip_display();
  }
}

static void draw_background(void) {
  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_draw_bitmap(galaxy->image, 0, 0, ALLEGRO_FLIP_HORIZONTAL);
}

static void refresh_game(void) {
  draw_background();
  show_lifes();
  show_score();
  update_ship();
  move_aliens();
  update_random_bombs();
  update_all_lasers();
  update_all_bombs();
  update_alien_hits();
  update_ship_hits();
  al_flip_display();
}

/*end util functions*/


/*ship updates*/
static bool is_ship_hit(Bomb* bomb) {
  return bomb->coordinate->x >= spaceShip->coordinate->x - (SPACE_SHIP_WIDTH / 2.0F) &&
         bomb->coordinate->x <= spaceShip->coordinate->x + (SPACE_SHIP_WIDTH / 2.0F) &&
         bomb->coordinate->y >= spaceShip->coordinate->y - SPACE_SHIP_HEIGHT &&
         bomb->coordinate->y <= WINDOW_HEIGHT;
}

static void update_ship_hits(void) {
  for (int i = 0; i < MAX_BOMBS; i++) {
    if (bombs[i] != NULL && is_ship_hit(bombs[i])) {
      al_draw_bitmap(spaceShip->destroyed_image, spaceShip->coordinate->x, spaceShip->coordinate->y, ALLEGRO_FLIP_HORIZONTAL);
      destroy_bomb(&bombs[i]);
      bombs[i] = NULL;
      show_dying_screen();
    }
  }
}

static void update_ship(void) {
  al_draw_bitmap(spaceShip->image, spaceShip->coordinate->x, spaceShip->coordinate->y, ALLEGRO_FLIP_HORIZONTAL);
  redraw_aliens();
  al_flip_display();
}

/*end ship updates*/

/*aliens*/

static bool can_move_right(float xCoordinate, float offset) {
  return xCoordinate <= al_get_display_width(display) - offset;
}

static bool can_move_left(float xCoordinate, float offset) {
  return xCoordinate >= offset;
}

static void redraw_aliens_move_down(void) {
  for (int i = 0; i < ALIEN_ROWS; i++) {
    for (int j = 0; j < ALIEN_COLS; j++) {
      if (aliens[i * ALIEN_COLS + j]->is_alive) {
        al_draw_bitmap(aliens[i * ALIEN_COLS + j]->image, aliens[i * ALIEN_COLS + j]->coordinate->x, aliens[i * ALIEN_COLS + j]->coordinate->y, ALLEGRO_FLIP_HORIZONTAL);
      }
    }
  }
}

static void redraw_aliens_move_up(void) {
  for (int i = 0; i < ALIEN_ROWS; i++) {
    for (int j = 0; j < ALIEN_COLS; j++) {
      if (aliens[i * ALIEN_COLS + j]->is_alive) {
       al_draw_bitmap(aliens[i * ALIEN_COLS + j]->image_move, aliens[i * ALIEN_COLS + j]->coordinate->x, aliens[i * ALIEN_COLS + j]->coordinate->y, ALLEGRO_FLIP_HORIZONTAL);
      }
    }
  }
}

static void redraw_aliens(void) {
  int timer_count = al_get_timer_count(timer);
  if (timer_count % 7 == 0) {
    redraw_aliens_move_up();
  } else {
    redraw_aliens_move_down();
  }
}

static void move_right(void) {
  if (rightTurn && can_move_right(aliens[ALIENS_AMOUNT - 1]->coordinate->x, SPACE_SHIP_WIDTH)) {
    for (int i = 0; i < ALIEN_ROWS; i++) {
      for (int j = 0; j < ALIEN_COLS; j++) {
        aliens[i * ALIEN_COLS + j]->coordinate->x += aliens[i * ALIEN_COLS + j]->velocity;
      }
    }
  } else {
    leftTurn = true;
    rightTurn = false;
  }
}

static void move_left(void) {
  if (leftTurn && can_move_left(aliens[0]->coordinate->x, LEFT_EDGE)) {
    for (int i = 0; i < ALIEN_ROWS; i++) {
      for (int j = 0; j < ALIEN_COLS; j++) {
        aliens[i * ALIEN_COLS + j]->coordinate->x -= aliens[i * ALIEN_COLS + j]->velocity;
      }
    }
  } else {
    rightTurn = true;
    leftTurn = false;
  }
}

static void move_down(void) {
  if (!can_move_left(aliens[0]->coordinate->x, LEFT_EDGE) || !can_move_right(aliens[ALIENS_AMOUNT - 1]->coordinate->x, SPACE_SHIP_WIDTH)) {
    for (int i = 0; i < ALIEN_ROWS; i++) {
      for (int j = 0; j < ALIEN_COLS; j++) {
        aliens[i * ALIEN_COLS + j]->coordinate->y += 5;
      }
    }
  }
}

static void move_aliens(void) {
  move_right();
  move_left();
  redraw_aliens();
  move_down();
}

static Alien* get_next_alien(void) {
  int randomIndex = random_next(0, ALIENS_AMOUNT);
  while (!isGameWon() && (aliens[randomIndex] == NULL || aliens[randomIndex]!= NULL && !aliens[randomIndex]->is_alive)) {
    randomIndex = random_next(0, ALIENS_AMOUNT);
  }
  return aliens[randomIndex];
}

static bool is_alien_hit(Alien* alien, LaserBeam* laserBeam) {
  return laserBeam->coordinate->x >= (alien->coordinate->x - (ALIEN_WIDTH / 1.5F)) &&
    laserBeam->coordinate->x <= (alien->coordinate->x + (ALIEN_WIDTH / 1.5F)) &&
    laserBeam->coordinate->y >= (alien->coordinate->y - (ALIEN_HEIGHT / 1.5F)) &&
    laserBeam->coordinate->y <= (alien->coordinate->y + (ALIEN_HEIGHT/ 1.5F));
}

static void update_alien_hits(void) {
  for (int i = 0; i < ALIEN_ROWS; i++) {
    for (int j = 0; j < ALIEN_COLS; j++) {
      for (int k = 0; k < MAX_LASERS; k++) {
        if (laserBeams[k] != NULL && aliens[i * ALIEN_COLS + j]->is_alive && is_alien_hit(aliens[i * ALIEN_COLS + j], laserBeams[k])) {
          aliens[i * ALIEN_COLS + j]->is_alive = false;
          spaceShip->score += aliens[i * ALIEN_COLS + j]->points; 
          remainingAliens--;
          al_draw_bitmap(aliens[i * ALIEN_COLS + j]->destroyed_image, aliens[i * ALIEN_COLS + j]->coordinate->x, aliens[i * ALIEN_COLS + j]->coordinate->y, ALLEGRO_FLIP_HORIZONTAL);
          destroy_laser_beam(&laserBeams[k]);
          laserBeams[k] = NULL;
        }
      }
    }
  }
}
/*end aliens*/


/*laserbeams*/

static void add_laser_beam(LaserBeam* laserBeam) {
  for (int i = 0; i < MAX_LASERS; i++) {
    if (laserBeams[i] == NULL) {
      laserBeams[i] = laserBeam;
      break;
    }
  }
}

static void update_laser_beam(LaserBeam* laserBeam) {
  if (laserBeam != NULL) {
      laserBeam->coordinate->y -= laserBeam->velocity;
      al_draw_bitmap(laserBeam->image, laserBeam->coordinate->x, laserBeam->coordinate->y, ALLEGRO_FLIP_HORIZONTAL);
  }
}

static void update_all_lasers(void) {
  for (int i = 0; i < MAX_LASERS; i++) {
    if (laserBeams[i] != NULL && laserBeams[i]->coordinate->y <= 0) {
      destroy_laser_beam(&laserBeams[i]);
      laserBeams[i] = NULL;
    } else {
      update_laser_beam(laserBeams[i]);
    }
  }
}
/*end laserbeams*/

/*bombs*/
static void add_bomb(Bomb* bomb) {
  for (int i = 0; i < MAX_BOMBS; i++) {
    if (bombs[i] == NULL) {
      bombs[i] = bomb;
      break;
    }
  }
}

static void update_bomb(Bomb* bomb) {
  if (bomb != NULL) {
    bomb->coordinate->y += bomb->velocity;
    al_draw_bitmap(bomb->image, bomb->coordinate->x, bomb->coordinate->y, ALLEGRO_FLIP_HORIZONTAL);
  }
}

static void update_all_bombs(void) {
  for (int i = 0; i < MAX_BOMBS; i++) {
    if (bombs[i] != NULL && bombs[i]->coordinate->y >= (WINDOW_HEIGHT - SPACE_SHIP_OFFSET)) {
      destroy_bomb(&bombs[i]);
      bombs[i] = NULL;
    } else {
      update_bomb(bombs[i]);
    }
  }
}

static void random_bomb(void) {
  Alien* alien = get_next_alien();
  if (alien != NULL) {
    Bomb* bomb = create_bomb(alien->coordinate->x + SPACE_SHIP_WIDTH / 2, alien->coordinate->y - SPACE_SHIP_HEIGHT / 2);
    add_bomb(bomb);
  }
}

static void update_random_bombs(void) {
  if (al_get_timer_count(timer) % BOMB_INTERVALL == 0) {
    random_bomb();
  }
}
/*end bombs*/