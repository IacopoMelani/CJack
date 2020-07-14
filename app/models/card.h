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

struct Card *card_new(unsigned int svalue, const char *symbol, const char *seed);

void card_info(struct Card *card, char *buf);

void card_print(struct Card *card);
