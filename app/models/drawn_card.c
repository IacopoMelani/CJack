#include "drawn_card.h"

#include "../../libs/mmalloc/alloc/mmalloc.h"

#define MAX_SCORE_LIMIT 21
#define CARD_LEN_TO_BLACKJACK 2

#define CONTEXT_DRAWN_CARD "drawn card"

#define CHECK_VALID_SCORE(head)                               \
    if (drawn_cards_total_score_raw(head) <= MAX_SCORE_LIMIT) \
    {                                                         \
        return;                                               \
    }

static void drawn_card_check_aces(struct DrawnCard *head);
static struct DrawnCard *drawn_card_init();
static unsigned int drawn_cards_total_score_raw(struct DrawnCard *head);

bool drawn_card_check_blackjack(struct DrawnCard *head)
{
    if (drawn_card_total_score(head) == MAX_SCORE_LIMIT && drawn_card_len(head) == CARD_LEN_TO_BLACKJACK)
    {
        return true;
    }

    return false;
}

void drawn_card_dealloc(struct DrawnCard *drawn_card)
{
    card_dealloc(drawn_card->card);
    mfree(drawn_card, CONTEXT_DRAWN_CARD);
}

unsigned int drawn_card_len(struct DrawnCard *head)
{
    unsigned int len;
    struct DrawnCard *pivot;

    pivot = head;

    while (pivot != NULL)
    {
        len++;
        pivot = pivot->next;
    }

    return len;
}

void drawn_card_push(struct DrawnCard **head, struct Card *card)
{
    struct DrawnCard **pivot;
    struct DrawnCard *node = NULL;

    node = drawn_card_init();
    node->card = card;

    pivot = head;
    while (*pivot != NULL)
    {
        pivot = &(*pivot)->next;
    }

    *pivot = node;
}

unsigned int drawn_card_total_score(struct DrawnCard *head)
{
    drawn_card_check_aces(head);
    return drawn_cards_total_score_raw(head);
}

static void drawn_card_check_aces(struct DrawnCard *head)
{
    CHECK_VALID_SCORE(head);

    struct DrawnCard *
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

static struct DrawnCard *drawn_card_init()
{
    struct DrawnCard *node = NULL;
    node = mmalloc(sizeof(struct DrawnCard), CONTEXT_DRAWN_CARD);
    if (node != NULL)
    {
        node->card = NULL;
        node->next = NULL;
    }
    return node;
}

static unsigned int drawn_cards_total_score_raw(struct DrawnCard *head)
{
    struct DrawnCard *pivot;
    unsigned int score = 0;

    pivot = head;

    while (pivot != NULL)
    {
        score += pivot->card->value;
        pivot = pivot->next;
    }

    return score;
}