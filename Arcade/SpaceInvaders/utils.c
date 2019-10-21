// file: src/utils.c
#include "utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <allegro5/allegro5.h>

void init_or_exit(bool is_initialized, const char *name) {
  if (!is_initialized) {
    printf("could not initialize %s: ERRNO %d\n", name, al_get_errno());
    exit(1);
  }
}

void* assert_not_null(void *pointer, const char *name) {
  if (!pointer) {
    printf( "could not create %s: ERRNO %d\n", name, al_get_errno());
    exit(1);
  }
  return pointer;
}

void assert_handler(const char *expression, const char *file, int line, const char *function) {
  printf("Expression (%s) failed on line %d of file %s in function %s\n",
      expression, line, file, function);
  assert(false);
}

int random_next(int lowerBound, int upperBound) {
  return lowerBound + rand() % (upperBound - lowerBound);
}

bool are_same_color(ALLEGRO_COLOR a, ALLEGRO_COLOR b) {
  return memcmp(&a, &b, sizeof(ALLEGRO_COLOR)) == 0;
}

bool is_in_range(int val, int min, int max) {
  return min <= val && val <= max;
}
