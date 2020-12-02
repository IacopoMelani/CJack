#ifndef PLAYER_GAME_H
#define PLAYER_GAME_H

#include "player.h"
#include "card.h"
#include "../../libs/mmalloc/types/types.h"

#define MAX_CARDS_FOR_DOUBLE 2

typedef struct PlayerGame
{
    PLAYER player;
    unsigned int amount_bet;
    bool is_cpu;
    struct PlayerGame *next;
} PlayerGame;

typedef PlayerGame *PLAYER_GAME;

bool player_game_can_double_down(PLAYER_GAME player_game);

bool player_game_bet_amount(PLAYER_GAME player_game, unsigned int amount_bet);

void player_game_dealloc(PLAYER_GAME player_game);

void player_game_drawn_card(PLAYER_GAME player_game, CARD card);

PLAYER_GAME player_game_init(PLAYER player, bool is_cpu);

void player_game_reset_amount_bet(PLAYER_GAME player_game);

#endif
