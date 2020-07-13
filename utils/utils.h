#pragma once

#include <stddef.h>

#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))

void shuffle_array(void *array, size_t n_elems, size_t elem_size);