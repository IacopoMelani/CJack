#include "player_game.h"

#include "../../libs/mmalloc/alloc/mmalloc.h"

#define CONTEXT_PLAYER_GAME "player game"

/*  Returns TRUE if player can double down */
BOOL player_game_can_double_down(PLAYER_GAME player_game)
{
    if (player_total_cards(player_game->player) > MAX_CARDS_FOR_DOUBLE)
    {
        return FALSE;
    }

    unsigned int actual_bet = player_game->amount_bet;

    if (actual_bet > player_bank_account(player_game->player))
    {
        return FALSE;
    }

    return TRUE;
}

BOOL player_game_bet_amount(PLAYER_GAME player_game, unsigned int amount_bet)
{
    BOOL res = player_bet_amount(player_game->player, amount_bet);
    if (!res)
    {
        return FALSE;
    }

    player_game->amount_bet += amount_bet;

    return TRUE;
}

void player_game_dealloc(PLAYER_GAME player_game)
{
    player_dealloc(player_game->player);
    mfree(player_game, CONTEXT_PLAYER_GAME);
}

void player_game_drawn_card(PLAYER_GAME player_game, CARD card)
{
    player_draw_card(player_game->player, card);
}

PLAYER_GAME player_game_init(PLAYER player, BOOL is_cpu)
{
    PLAYER_GAME player_game = mmalloc(sizeof(struct PlayerGame), CONTEXT_PLAYER_GAME);

    if (player_game != NULL)
    {
        player_game->player = player;
        player_game->amount_bet = 0;
        player_game->is_cpu = is_cpu;
        player_game->next = NULL;
    }

    return player_game;
}

void player_game_reset_amount_bet(PLAYER_GAME player_game)
{
    player_game->amount_bet = 0;
}
