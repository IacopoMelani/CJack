#pragma once

#include <stddef.h>

#include "../types/types.h"
#include "../registry/registry.h"

/* free and print deallocated context */
extern void mfree(void *ptr, unused const char *context);

/* calloc and print allocated context and size */
extern void *mcalloc(size_t nItems, size_t size, unused const char *context);

/* malloc and print allocated context and size */
extern void *mmalloc(size_t size, unused const char *context);

/* realloc and print allocated context and size */
extern void *mrealloc(void *ptr, size_t size, unused const char *context);