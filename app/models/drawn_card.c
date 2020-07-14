#include "drawn_card.h"

#include "../../libs/mmalloc/alloc/mmalloc.h"

#define MAX_SCORE_LIMIT 21

static void drawn_card_check_aces(struct DrawnCard *head);
static unsigned int drawn_cars_total_score_raw(struct DrawnCard *head);

bool drawn_card_check_blackjack(struct DrawnCard *head)
{
    if (drawn_card_total_score(head) == MAX_SCORE_LIMIT)
    {
        return true;
    }

    return false;
}

struct DrawnCard *drawn_card_init()
{
    struct DrawnCard *head = mmalloc(sizeof(struct DrawnCard), "drawn card");
    return head;
}

void drawn_card_push(struct DrawnCard *head, struct Card *card)
{
    struct DrawnCard *pivot;

    pivot = head;
    while (pivot->next != NULL)
    {
        pivot = pivot->next;
    }

    pivot->card = card;
}

unsigned int drawn_card_total_score(struct DrawnCard *head)
{
    drawn_card_check_aces(head);
    return drawn_cars_total_score_raw(head);
}

static void drawn_card_check_aces(struct DrawnCard *head)
{
    if (drawn_cars_total_score_raw(head) <= MAX_SCORE_LIMIT)
    {
        return;
    }

    struct DrawnCard *pivot;

    pivot = head;
    while (pivot != NULL)
    {
        if (drawn_cars_total_score_raw(head) <= MAX_SCORE_LIMIT)
        {
            return;
        }

        if (card_is_ace(pivot->card))
        {
            pivot->card->value = 1;
        }

        pivot = pivot->next;
    }
}

static unsigned int drawn_cars_total_score_raw(struct DrawnCard *head)
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