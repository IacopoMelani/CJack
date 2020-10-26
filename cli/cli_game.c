#include "cli_game.h"

#include "../app/models/dealer.h"
#include "../libs/mmalloc/alloc/mmalloc.h"

struct CliGame
{
    DEALER dealer;
};

void cli_game_dealloc(CLI_GAME cli_game)
{
    dealer_dealloc(cli_game->dealer);
    mfree(cli_game, "cli_game");
}

CLI_GAME cli_game_init()
{
    CLI_GAME cli_game = mmalloc(sizeof(struct CliGame), "cli_game");
    DEALER dealer = dealer_init();

    cli_game->dealer = dealer;

    return cli_game;
}

void cli_game_play(CLI_GAME cli_game)
{
    dealer_play(cli_game->dealer);
}
