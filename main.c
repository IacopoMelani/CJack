
#include "app/models/dealer.h"
#include "libs/mmalloc/registry/registry.h"
#include "app/utils/utils.h"

int main()
{
    dealer_init();
    dealer_play();

#ifdef DEBUG
    registry_print_alloc();
#endif

    return 0;
}