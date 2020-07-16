#pragma once

#include <stddef.h>
#include "../../libs/mmalloc/types/types.h"

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

void card_info(struct Card *card, char *buf);

struct Card *card_init(unsigned int svalue, const char *symbol, const char *seed);

bool card_is_ace(struct Card *card);

void card_print(struct Card *card);