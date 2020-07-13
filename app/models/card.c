#include "card.h"

#include <stdio.h>

#include "../../utils/utils.h"

#include "../../libs/mmalloc/alloc/mmalloc.h"

unsigned int allValue[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};

const char *allSeeds[] = {"diamonds", "club", "hearts", "spades"};

const char *allSymbols[] = {"Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Height", "Nine", "Ten", "Jack", "Queen", "King"};

size_t allValue_count = NELEMS(allValue);
size_t allSeeds_count = NELEMS(allSeeds);
size_t allSymbols_count = NELEMS(allSymbols);

struct Card *card_new(unsigned int value, const char *symbol, const char *seed)
{
    char buf[50];
    sprintf(buf, "%s of %s", symbol, seed);
    struct Card *c = (struct Card *)mmalloc(sizeof(struct Card), buf);
    if (c == NULL)
    {
        return NULL;
    }

    c->value = value;
    c->symbol = symbol;
    c->seed = seed;

    return c;
}