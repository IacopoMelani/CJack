
#include "app/models/dealer.h"
#include "libs/mmalloc/registry/registry.h"

#ifndef DEBUG
#define DEBUG 1
#endif

int main()
{
    dealer_init();
    dealer_play();

#if DEBUG
    registry_print_alloc();
#endif

    return 0;
}