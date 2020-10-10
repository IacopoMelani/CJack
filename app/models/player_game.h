#ifndef PLAYER_GAME_H
#define PLAYER_GAME_H

#include "player.h"
#include "card.h"
#include "../../libs/mmalloc/types/types.h"

typedef struct PlayerGame
{
    PLAYER player;
    unsigned int amount_bet;
    bool is_cpu;
    struct PlayerGame *next;
} PlayerGame;

typedef PlayerGame *PLAYER_GAME;

void player_game_dealloc(PLAYER_GAME player_game);

void player_game_drawn_card(PLAYER_GAME player_game, CARD card);

PLAYER_GAME player_game_init(PLAYER player, bool is_cpu);

#endif
