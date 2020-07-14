#pragma once

#include <stddef.h>

extern unsigned int allValue[];
extern const char *allSeeds[];
extern const char *allSymbols[];

extern size_t allValue_count;
extern size_t allSeeds_count;
extern size_t allSymbols_count;

struct Card
{
    unsigned int value;
    const char *symbol;
    const char *seed;
};

struct DrawnCard
{
    struct Card *card;
    struct DrawnCard *next;
};

struct Card *card_new(unsigned int svalue, const char *symbol, const char *seed);

void card_info(struct Card *card, char *buf);

void card_print(struct Card *card);

struct DrawnCard *drawn_card_init();

void drawn_card_push(struct DrawnCard *head, struct Card *card);

