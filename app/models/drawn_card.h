#ifndef DRAWN_CARD_H
#define DRAWN_CARD_H

#include "card.h"

typedef struct DrawnCard
{
    CARD card;
    struct DrawnCard *next;
} DrawnCard;

typedef DrawnCard *DRAWN_CARD;

bool drawn_card_check_blackjack(DRAWN_CARD head);

void drawn_card_dealloc(DRAWN_CARD drawn_card);

unsigned int drawn_card_len(DRAWN_CARD head);

void drawn_card_print(DRAWN_CARD drawn_card);

void drawn_card_push(DRAWN_CARD *head, CARD card);

unsigned int drawn_card_total_cards(DRAWN_CARD head);

unsigned int drawn_card_total_score(DRAWN_CARD head);

#endif
