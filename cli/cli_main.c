#include <time.h>
#include <stdlib.h>

#include "cli_game.h"
#include "../utils/utils.h"
#include "../libs/mmalloc/registry/registry.h"

int main()
{
    srand(time(NULL));

    clear_screen();
    CLI_GAME cli_game = cli_game_init();
    cli_game_play(cli_game);
    cli_game_dealloc(cli_game);

#ifdef DEBUG
    registry_print_alloc();
#endif

    return 0;
}