#include "card.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../utils/utils.h"
#include "../../libs/mmalloc/alloc/mmalloc.h"

#define CONTEXT_CARD "card"

#define SYMBOL_ACE "Ace"

unsigned int allValue[] = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};

const char *allSeeds[] = {"diamonds", "club", "hearts", "spades"};

const char *allSymbols[] = {SYMBOL_ACE, "Two", "Three", "Four", "Five", "Six", "Seven", "Height", "Nine", "Ten", "Jack", "Queen", "King"};

size_t allValue_count = NELEMS(allValue);
size_t allSeeds_count = NELEMS(allSeeds);
size_t allSymbols_count = NELEMS(allSymbols);

void card_dealloc(CARD card)
{
    char buf[80];
    card_sprintf(buf, card);
    mfree(card, buf);
}

CARD card_init(unsigned int value, const char *symbol, const char *seed)
{
    char buf[80];
    struct Card bufCard = {.seed = seed, .value = value, .symbol = symbol};
    card_sprintf(buf, &bufCard);
    CARD c = mmalloc(sizeof(struct Card), buf);

    c->value = value;
    c->symbol = symbol;
    c->seed = seed;

    return c;
}

BOOL card_is_ace(const CARD card)
{
    BOOL valid = FALSE;

    if (strcmp(card->symbol, SYMBOL_ACE) == 0)
    {
        valid = TRUE;
    }

    return valid;
}

void card_print(const CARD card)
{
    printf("%s of %s\n", card->symbol, card->seed);
}

void card_sprintf(char *buf, const CARD card)
{
    sprintf(buf, "%s of %s", card->symbol, card->seed);
}