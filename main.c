#include <time.h>
#include <stdlib.h>

#include "app/models/dealer.h"
#include "libs/mmalloc/registry/registry.h"
#include "utils/utils.h"

int main()
{
    srand(time(NULL));

    clear_screen();
    DEALER dealer = dealer_init();
    dealer_play(dealer);
    dealer_dealloc(dealer);

#ifdef DEBUG
    registry_print_alloc();
#endif

    return 0;
}