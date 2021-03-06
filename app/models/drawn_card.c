#include "drawn_card.h"

#include "../../libs/mmalloc/alloc/mmalloc.h"

#define MAX_SCORE_LIMIT 21
#define CARD_LEN_TO_BLACKJACK 2

#define CONTEXT_DRAWN_CARD "drawn card"

#define CHECK_VALID_SCORE(head)                              \
    if (drawn_card_total_score_raw(head) <= MAX_SCORE_LIMIT) \
    {                                                        \
        return;                                              \
    }

static void drawn_card_check_aces(DRAWN_CARD head);
static DRAWN_CARD drawn_card_init();
static unsigned int drawn_card_total_score_raw(const DRAWN_CARD head);

BOOL drawn_card_check_blackjack(DRAWN_CARD head)
{
    if (drawn_card_total_score(head) == MAX_SCORE_LIMIT && drawn_card_len(head) == CARD_LEN_TO_BLACKJACK)
    {
        return TRUE;
    }

    return FALSE;
}

void drawn_card_dealloc(DRAWN_CARD drawn_card)
{
    card_dealloc(drawn_card->card);
    mfree(drawn_card, CONTEXT_DRAWN_CARD);
}

unsigned int drawn_card_len(const DRAWN_CARD head)
{
    unsigned int len;
    DRAWN_CARD pivot;

    pivot = head;

    while (pivot != NULL)
    {
        len++;
        pivot = pivot->next;
    }

    return len;
}

void drawn_card_print(const DRAWN_CARD drawn_card)
{
    DRAWN_CARD pivot;

    pivot = drawn_card;
    while (pivot != NULL)
    {
        card_print(pivot->card);
        pivot = pivot->next;
    }
}

void drawn_card_push(DRAWN_CARD *head, CARD card)
{
    DRAWN_CARD *pivot;
    DRAWN_CARD node = NULL;

    node = drawn_card_init();
    node->card = card;

    pivot = head;
    while (*pivot != NULL)
    {
        pivot = &(*pivot)->next;
    }

    *pivot = node;
}

unsigned int drawn_card_total_cards(const DRAWN_CARD head)
{
    DRAWN_CARD pivot;

    unsigned int tot = 0;

    pivot = head;
    while (pivot != NULL)
    {
        tot++;
        pivot = pivot->next;
    }

    return tot;
}

unsigned int drawn_card_total_score(DRAWN_CARD head)
{
    drawn_card_check_aces(head);
    return drawn_card_total_score_raw(head);
}

static void drawn_card_check_aces(DRAWN_CARD head)
{
    CHECK_VALID_SCORE(head);

    DRAWN_CARD
    pivot;

    pivot = head;
    while (pivot != NULL)
    {
        CHECK_VALID_SCORE(head);

        if (card_is_ace(pivot->card))
        {
            pivot->card->value = 1;
        }

        pivot = pivot->next;
    }
}

static DRAWN_CARD drawn_card_init()
{
    DRAWN_CARD node = NULL;
    node = mmalloc(sizeof(struct DrawnCard), CONTEXT_DRAWN_CARD);
    if (node != NULL)
    {
        node->card = NULL;
        node->next = NULL;
    }
    return node;
}

static unsigned int drawn_card_total_score_raw(const DRAWN_CARD head)
{
    DRAWN_CARD pivot;
    unsigned int score = 0;

    pivot = head;

    while (pivot != NULL)
    {
        score += pivot->card->value;
        pivot = pivot->next;
    }

    return score;
}