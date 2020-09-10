#ifndef DECK_H
#define DECK_H

#include "card.h"

#define DECK_LEN 52

struct Deck;

void deck_dealloc(struct Deck *);
struct Card *deck_draw_card(struct Deck *);
struct Deck *deck_init();

#endif
