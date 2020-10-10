#ifndef DECK_H
#define DECK_H

#include "card.h"

#define DECK_LEN 52

typedef struct Deck Deck;

typedef Deck *DECK;

void deck_dealloc(DECK);
CARD deck_draw_card(DECK);
DECK deck_init();

#endif
