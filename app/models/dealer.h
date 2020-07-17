#pragma once

#include "player_game.h"

#define MIN_SCORE_DEALER_STOP 17

void dealer_add_player_game(struct PlayerGame *player_game);

void dealer_init();

void dealer_play();

void dealer_print_initial_cards();