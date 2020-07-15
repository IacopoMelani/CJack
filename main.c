
#include "app/models/deck.h"
#include "libs/mmalloc/registry/registry.h"

#ifndef DEBUG
#define DEBUG 1
#endif

int main()
{
    deck_init();

#if DEBUG
    registry_print_alloc();
#endif

    return 0;
}