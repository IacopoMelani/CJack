#ifndef CARD_H
#define CARD_H

#include <stddef.h>
#include "../../libs/mmalloc/types/types.h"

extern unsigned int allValue[];
extern const char *allSeeds[];
extern const char *allSymbols[];

extern size_t allValue_count;
extern size_t allSeeds_count;
extern size_t allSymbols_count;

typedef struct Card
{
    unsigned int value;
    const char *symbol;
    const char *seed;
} Card;

typedef Card *CARD;

void card_dealloc(CARD card);

CARD card_init(unsigned int svalue, const char *symbol, const char *seed);

bool card_is_ace(CARD card);

void card_print(CARD card);

void card_sprintf(char *buf, CARD card);

#endif
