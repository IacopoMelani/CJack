#include "deck.h"

#include "card.h"
#include "../../utils/utils.h"
#include "../../libs/mmalloc/alloc/mmalloc.h"

struct Card *deck;

static void deck_init_cards(struct Card **deck);

void deck_init()
{
    deck = mmalloc(DECK_LEN * sizeof(struct Card), "deck");
    deck_init_cards(&deck);
}

static void deck_init_cards(struct Card **deck)
{
    int cardPos = 0;
    struct Card *tmpCard;

    for (unsigned int i = 0; i < allSeeds_count; i++)
    {
        for (unsigned int k = 0; k < allValue_count; k++)
        {
            tmpCard = card_new(allValue[k], allSymbols[k], allSeeds[i]);
            deck[cardPos] = tmpCard;
            cardPos++;
        }
    }
}