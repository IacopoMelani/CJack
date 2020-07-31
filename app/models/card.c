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

static void card_sprintf(char *buf, struct Card *card);

void card_dealloc(struct Card *card)
{
    char buf[80];
    card_sprintf(buf, card);
    mfree(card, buf);
}

void card_info(struct Card *card, char *buf)
{
    sprintf(buf, "%s of %s", card->symbol, card->seed);
}

struct Card *card_init(unsigned int value, const char *symbol, const char *seed)
{
    char buf[80];
    struct Card bufCard = {.seed = seed, .value = value, .symbol = symbol};
    card_sprintf(buf, &bufCard);
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

static void card_sprintf(char *buf, struct Card *card)
{
    sprintf(buf, "%s of %s", card->symbol, card->seed);
}