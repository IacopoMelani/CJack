#ifndef DEALER_H
#define DEALER_H

#include "player_game.h"

#define MIN_SCORE_DEALER_STOP 17

#define MAX_VALID_SCORE 21

#define MAX_CARDS_FOR_DOUBLE 2

struct Dealer;

void dealer_add_player_game(struct Dealer *, struct PlayerGame *);

struct Dealer *dealer_init();

void dealer_play(struct Dealer *);

void dealer_print_initial_cards(struct Dealer *);

#endif
