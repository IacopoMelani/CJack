#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

#define BREAK_LINE printf("\n");

#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))

#ifndef unused
#define unused __attribute__((unused))
#endif

void clear_screen();

void shuffle_array(void *array, size_t n, size_t size);

#endif
