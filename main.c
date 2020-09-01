
#include "app/models/dealer.h"
#include "libs/mmalloc/registry/registry.h"
#include "app/utils/utils.h"

#ifndef DEBUG
#define DEBUG 0
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