#include "player_game.h"

#include "../../libs/mmalloc/alloc/mmalloc.h"

#define CONTEXT_PLAYER_GAME "player game"

void player_game_dealloc(struct PlayerGame *player_game)
{
    player_dealloc(player_game->player);
    mfree(player_game, CONTEXT_PLAYER_GAME);
}

void player_game_drawn_card(struct PlayerGame *player_game, struct Card *card)
{
    player_draw_card(player_game->player, card);
}

struct PlayerGame *player_game_init(struct Player *player, bool is_cpu)
{
    struct PlayerGame *player_game = mmalloc(sizeof(struct PlayerGame), CONTEXT_PLAYER_GAME);

    if (player_game != NULL)
    {
        player_game->player = player;
        player_game->amount_bet = 0;
        player_game->is_cpu = is_cpu;
        player_game->next = NULL;
    }

    return player_game;
}
