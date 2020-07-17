#include "dealer.h"

#include <stdio.h>

#include "drawn_card.h"
#include "deck.h"
#include "player_game.h"

#include "../../libs/mmalloc/alloc/mmalloc.h"

#define CONTEXT_DEALER "dealer"

struct Dealer
{
    struct DrawnCard *cards;
    struct PlayerGame *player_game;
};

static struct Dealer *dealer;

static void dealer_draw_card(struct Card *card);

void dealer_add_player_game(struct PlayerGame *player_game)
{
    struct PlayerGame *pivot;

    pivot = dealer->player_game;

    while (pivot != NULL)
    {
        pivot = pivot->next;
    }

    pivot = player_game;
}

void dealer_init()
{
    if (dealer == NULL)
    {
        dealer = mmalloc(sizeof(struct Dealer), CONTEXT_DEALER);
    }
}

void dealer_play()
{
    while (drawn_card_total_score(dealer->cards) < MIN_SCORE_DEALER_STOP)
    {
        dealer_draw_card(deck_draw_card());
    }
}

void dealer_print_initial_cards()
{
    struct DrawnCard *pivot;
    int cont = 0;

    pivot = dealer->cards;

    while (pivot != NULL)
    {
        if (cont == 0)
        {
            printf("card face down\n");
        }
        else
        {
            card_print(pivot->card);
        }

        pivot = pivot->next;
        cont++;
    }
}

static void dealer_draw_card(struct Card *card)
{
    drawn_card_push(dealer->cards, card);
}