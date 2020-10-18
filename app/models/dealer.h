#ifndef DEALER_H
#define DEALER_H

#include "player_game.h"

#define MIN_SCORE_DEALER_STOP 17

#define MAX_VALID_SCORE 21

#define MAX_CARDS_FOR_DOUBLE 2

typedef struct Dealer Dealer;

typedef Dealer *DEALER;

void dealer_add_player_game(DEALER, PLAYER_GAME);

void dealer_dealloc(DEALER);

DEALER dealer_init();

void dealer_play(DEALER);

void dealer_print_initial_cards(const DEALER);

#endif
