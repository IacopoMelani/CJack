#include "deck.h"

#include <stdlib.h>

#include "card.h"
#include "../utils/utils.h"
#include "../../libs/mmalloc/alloc/mmalloc.h"

#define SHUFFLE_TIMES 5

struct Card **deck;

static void deck_init_cards(struct Card **deck);

void deck_init()
{
    deck = mmalloc(DECK_LEN * sizeof(struct Card), "deck");
    deck_init_cards(deck);

    deck_shuffle();

    for (int i = 0; i < DECK_LEN; i++)
    {
        card_print(deck[i]);
        mfree(deck[i], "card");
    }
    mfree(deck, "deck");
}

void deck_shuffle()
{
    for (int i = 0; i < SHUFFLE_TIMES; i++)
    {
        shuffle_array(deck, DECK_LEN, sizeof(deck[0]));
    }
}

static void deck_init_cards(struct Card **d)
{
    int cardPos = 0;
    for (unsigned int i = 0; i < allSeeds_count; i++)
    {
        for (unsigned int k = 0; k < allValue_count; k++)
        {
            struct Card *tmpCard = card_new(allValue[k], allSymbols[k], allSeeds[i]);
            d[cardPos] = tmpCard;
            cardPos++;
        }
    }
}