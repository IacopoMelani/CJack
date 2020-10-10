#include "deck.h"

#include <stdlib.h>

#include "card.h"
#include "../utils/utils.h"
#include "../../libs/mmalloc/alloc/mmalloc.h"

#ifndef SHUFFLE_TIMES
#define SHUFFLE_TIMES 10
#endif

struct DeckStack
{
    CARD card;
    struct DeckStack *next;
};

struct Deck
{
    struct DeckStack *deck_stack;
};

static void deck_arr_dealloc(CARD *);
static void deck_arr_init_cards(CARD *d);
static void deck_arr_shuffle(CARD *);
static void deck_stack_dealloc(struct DeckStack *);
static struct DeckStack *deck_stack_init(CARD *);

void deck_dealloc(DECK deck)
{
    deck_stack_dealloc(deck->deck_stack);
    mfree(deck, "deck");
}

CARD deck_draw_card(DECK deck)
{
    CARD card = NULL;

    if (deck->deck_stack != NULL)
    {
        struct DeckStack *old_head = deck->deck_stack;

        card = deck->deck_stack->card;
        deck->deck_stack = deck->deck_stack->next;
        mfree(old_head, "deck stack");
    }

    return card;
}

DECK deck_init()
{
    DECK deck = mmalloc(sizeof(struct Deck), "deck");
    CARD *deck_arr = mmalloc(DECK_LEN * sizeof(struct Card), "deck arr");
    deck_arr_init_cards(deck_arr);
    deck_arr_shuffle(deck_arr);
    deck->deck_stack = deck_stack_init(deck_arr);
    deck_arr_dealloc(deck_arr);
    return deck;
}

static void deck_arr_dealloc(CARD *deck_arr)
{
    if (deck_arr != NULL)
    {
        mfree(deck_arr, "deck arr");
    }
}

static void deck_arr_init_cards(CARD *d)
{
    int cardPos = 0;
    for (unsigned int i = 0; i < allSeeds_count; i++)
    {
        for (unsigned int k = 0; k < allValue_count; k++)
        {
            CARD tmpCard = card_init(allValue[k], allSymbols[k], allSeeds[i]);
            d[cardPos] = tmpCard;
            cardPos++;
        }
    }
}

static void deck_arr_shuffle(CARD *deck_arr)
{
    for (int i = 0; i < SHUFFLE_TIMES; i++)
    {
        shuffle_array(deck_arr, DECK_LEN, sizeof(deck_arr[0]));
    }
}

static void deck_stack_dealloc(struct DeckStack *deck_stack)
{
    struct DeckStack *pivot = deck_stack;
    struct DeckStack *old;

    while (pivot != NULL)
    {
        old = pivot;
        card_dealloc(pivot->card);
        pivot = pivot->next;
        mfree(old, "deck stack");
    }
}

static struct DeckStack *deck_stack_init(CARD *deck_arr)
{
    struct DeckStack *deck_stack = mmalloc(sizeof(struct DeckStack), "deck stack");

    struct DeckStack *pivot;
    CARD tmp;

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

    return deck_stack;
}