#include "card.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/utils.h"
#include "../../libs/mmalloc/alloc/mmalloc.h"

#define CONTEXT_CARD "card"

#define SYMBOL_ACE "Ace"

unsigned int allValue[] = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};

const char *allSeeds[] = {"diamonds", "club", "hearts", "spades"};

const char *allSymbols[] = {SYMBOL_ACE, "Two", "Three", "Four", "Five", "Six", "Seven", "Height", "Nine", "Ten", "Jack", "Queen", "King"};

size_t allValue_count = NELEMS(allValue);
size_t allSeeds_count = NELEMS(allSeeds);
size_t allSymbols_count = NELEMS(allSymbols);

void card_dealloc(struct Card *card)
{
    mfree(card, CONTEXT_CARD);
}

void card_info(struct Card *card, char *buf)
{
    sprintf(buf, "%s of %s", card->symbol, card->seed);
}

struct Card *card_init(unsigned int value, const char *symbol, const char *seed)
{
    struct Card *c = (struct Card *)mmalloc(sizeof(struct Card), CONTEXT_CARD);
    if (c == NULL)
    {
        return NULL;
    }

    c->value = value;
    c->symbol = symbol;
    c->seed = seed;

    return c;
}

bool card_is_ace(struct Card *card)
{
    if (strcmp(card->symbol, SYMBOL_ACE) == 0)
    {
        return true;
    }

    return false;
}

void card_print(struct Card *card)
{
    printf("%s of %s\n", card->symbol, card->seed);
}
