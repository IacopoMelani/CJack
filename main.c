
#include "app/models/dealer.h"
#include "libs/mmalloc/registry/registry.h"
#include "app/utils/utils.h"

int main()
{
    struct Dealer *dealer = dealer_init();
    dealer_play(dealer);

#ifdef DEBUG
    registry_print_alloc();
#endif

    return 0;
}