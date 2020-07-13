
#include "app/models/deck.h"
#include "libs/mmalloc/registry/registry.h"

int main()
{
    deck_init();

    registry_print_alloc();

    return 0;
}