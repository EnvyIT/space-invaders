#ifndef utils_h
#define utils_h
#endif // !

#include <stdio.h>
#include <stdbool.h>

void init_or_exit(bool is_initialized, const char *name);
void* assert_not_null(void *ptr, const char *name);
void assert_handler(const char *expr, const char *file, int line, const char *func);
int random_next(int lowerBound, int upperBound);