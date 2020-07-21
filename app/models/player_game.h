#pragma once

#include "player.h"
#include "card.h"
#include "../../libs/mmalloc/types/types.h"

struct PlayerGame
{
    struct Player *player;
    unsigned int amount_bet;
    bool is_cpu;
    struct PlayerGame *next;
};

void player_game_dealloc(struct PlayerGame *player_game);

void player_game_drawn_card(struct PlayerGame *player_game, struct Card *card);

struct PlayerGame *player_game_init(struct Player *player, bool is_cpu);