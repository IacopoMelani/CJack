#ifndef DEALER_H
#define DEALER_H

#include "deck.h"
#include "player_game.h"

#define MIN_SCORE_DEALER_STOP 17

#define MAX_VALID_SCORE 21

typedef struct Dealer Dealer;

typedef Dealer *DEALER;

void dealer_add_player_game(DEALER, PLAYER_GAME);

void dealer_dealloc(DEALER);

void dealer_dealloc_drawn_card_and_deck(DEALER);

void dealer_draw_card(DEALER, CARD);

DECK dealer_get_deck(DEALER);

DRAWN_CARD dealer_get_drawn_card(DEALER);

PLAYER_GAME dealer_get_player_game(DEALER);

DEALER dealer_init();

void dealer_init_deck(DEALER);

void dealer_print_initial_cards(const DEALER);

#endif
