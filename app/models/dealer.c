#include "dealer.h"

#include <stdio.h>
#include <string.h>

#include "drawn_card.h"
#include "deck.h"
#include "player_game.h"

#include "../../libs/mmalloc/alloc/mmalloc.h"
#include "../../utils/utils.h"

#define CONTEXT_DEALER "dealer"

struct Dealer
{
    DECK deck;
    DRAWN_CARD cards;
    PLAYER_GAME player_game;
};

static void dealer_dealloc_deck(DEALER);
static void dealer_dealloc_drawn_cards(DEALER);
static void dealer_dealloc_player_game(DEALER);

void dealer_add_player_game(DEALER dealer, PLAYER_GAME player_game)
{
    PLAYER_GAME *pivot;

    pivot = &dealer->player_game;

    while (*pivot != NULL)
    {
        pivot = &(*pivot)->next;
    }

    *pivot = player_game;
}

void dealer_dealloc(DEALER dealer)
{
    dealer_dealloc_drawn_cards(dealer);
    dealer_dealloc_player_game(dealer);
    mfree(dealer, CONTEXT_DEALER);
}

void dealer_dealloc_drawn_card_and_deck(DEALER dealer)
{
    dealer_dealloc_drawn_cards(dealer);
    dealer_dealloc_deck(dealer);
}

void dealer_draw_card(DEALER dealer, CARD card)
{
    drawn_card_push(&dealer->cards, card);
}

inline DECK dealer_get_deck(DEALER dealer)
{
    return dealer->deck;
}

inline DRAWN_CARD dealer_get_drawn_card(DEALER dealer)
{
    return dealer->cards;
}

inline PLAYER_GAME dealer_get_player_game(DEALER dealer)
{
    return dealer->player_game;
}

DEALER dealer_init()
{

    DEALER dealer = mmalloc(sizeof(Dealer), CONTEXT_DEALER);

    dealer->cards = NULL;
    dealer->player_game = NULL;

    return dealer;
}

void dealer_init_deck(DEALER dealer)
{
    dealer->deck = deck_init();
}

void dealer_print_initial_cards(const DEALER dealer)
{
    DRAWN_CARD pivot;
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

static void dealer_dealloc_deck(DEALER dealer)
{
    deck_dealloc(dealer->deck);
    dealer->deck = NULL;
}

static void dealer_dealloc_drawn_cards(DEALER dealer)
{
    DRAWN_CARD pivot, old_node;

    pivot = dealer->cards;
    while (pivot != NULL)
    {
        old_node = pivot;
        pivot = pivot->next;
        drawn_card_dealloc(old_node);
    }

    dealer->cards = NULL;
}

static void dealer_dealloc_player_game(DEALER dealer)
{
    PLAYER_GAME pivot, old_node;

    pivot = dealer->player_game;
    while (pivot != NULL)
    {
        old_node = pivot;
        pivot = pivot->next;
        player_game_dealloc(old_node);
    }
}
