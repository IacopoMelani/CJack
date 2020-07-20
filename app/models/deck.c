#include "deck.h"

#include <stdlib.h>

#include "card.h"
#include "../utils/utils.h"
#include "../../libs/mmalloc/alloc/mmalloc.h"

#define SHUFFLE_TIMES 5

struct DeckStack
{
    struct Card *card;
    struct DeckStack *next;
};

static void deck_arr_dealloc();
static void deck_arr_init_cards(struct Card **d);
static void deck_arr_shuffle();
static void deck_stack_dealloc();
static void deck_stack_init();

struct Card **deck_arr;
struct DeckStack *deck_stack;

struct Card *deck_draw_card()
{
    struct Card *card = NULL;

    if (deck_stack != NULL)
    {
        struct DeckStack *old_head = deck_stack;

        card = deck_stack->card;
        deck_stack = deck_stack->next;
        mfree(old_head, "deck stack");
    }

    return card;
}

void deck_init()
{
    deck_arr_dealloc();
    deck_stack_dealloc();

    deck_arr = mmalloc(DECK_LEN * sizeof(struct Card), "deck arr");
    deck_arr_init_cards(deck_arr);
    deck_arr_shuffle();
    deck_stack_init();
    // for (int i = 0; i < DECK_LEN; i++)
    // {
    //     mfree(deck_draw_card(), "card");
    // }
    // deck_stack_dealloc();
}

static void deck_arr_dealloc()
{
    if (deck_arr != NULL)
    {
        mfree(deck_arr, "deck arr");
    }
}

static void deck_arr_init_cards(struct Card **d)
{
    int cardPos = 0;
    for (unsigned int i = 0; i < allSeeds_count; i++)
    {
        for (unsigned int k = 0; k < allValue_count; k++)
        {
            struct Card *tmpCard = card_init(allValue[k], allSymbols[k], allSeeds[i]);
            d[cardPos] = tmpCard;
            cardPos++;
        }
    }
}

static void deck_arr_shuffle()
{
    for (int i = 0; i < SHUFFLE_TIMES; i++)
    {
        shuffle_array(deck_arr, DECK_LEN, sizeof(deck_arr[0]));
    }
}

static void deck_stack_dealloc()
{
    struct DeckStack *pivot = deck_stack;
    struct DeckStack *old;

    while (pivot != NULL)
    {
        old = pivot;
        mfree(pivot->card, "card");
        pivot = pivot->next;
        mfree(old, "deck stack");
    }
}

static void deck_stack_init()
{
    deck_stack = mmalloc(sizeof(struct DeckStack), "deck stack");

    struct DeckStack *pivot;
    struct Card *tmp;

    pivot = deck_stack;

    for (int i = DECK_LEN - 1; i >= 0; i--)
    {
        tmp = deck_arr[i];
        pivot->card = tmp;
        if (i != 0)
        {
            pivot->next = mmalloc(sizeof(struct DeckStack), "deck stack");
            pivot = pivot->next;
        }
    }

    deck_arr_dealloc();
}