#pragma once

#include "card.h"

struct DrawnCard
{
    struct Card *card;
    struct DrawnCard *next;
};

bool drawn_card_check_blackjack(struct DrawnCard *head);

void drawn_card_push(struct DrawnCard *head, struct Card *card);

unsigned int drawn_card_total_score(struct DrawnCard *head);
