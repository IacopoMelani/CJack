#ifndef REGISTRY_H
#define REGISTRY_H

#include <stddef.h>

#include "../types/types.h"

/* clears the registry alloc */
extern void registry_clear_alloc(void *mem_alloc_ptr);

/* returns the current legth of the registry alloc */
extern unsigned int registry_len(void);

/* returns the total size allocated */
extern size_t registry_total_size(void);

/* prints the complete information about actual registry alloc */
extern void registry_print_alloc(void);

/* writes to registry alloc a new allocation */
extern BOOL registry_write_alloc(size_t size, void *mem_alloc_ptr, const char *context);
#endif
