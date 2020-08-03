#include "dealer.h"

#include <stdio.h>
#include <string.h>

#include "drawn_card.h"
#include "deck.h"
#include "player_game.h"

#include "../../libs/mmalloc/alloc/mmalloc.h"

#ifndef DEBUG
#define DEBUG 0
#endif

#define CONTEXT_DEALER "dealer"

struct Dealer
{
    struct DrawnCard *cards;
    struct PlayerGame *player_game;
};

static struct Dealer *dealer;

static void add_player(struct PlayerGame **head, struct PlayerGame *player_game);
static void dealloc();
static void dealloc_drawn_cards();
static void dealloc_player_game();
static void draw_card(struct Card *card);
static void init_players();

void dealer_add_player_game(struct PlayerGame *player_game)
{
    add_player(&dealer->player_game, player_game);
}

void dealer_init()
{
    if (dealer == NULL)
    {
        dealer = mmalloc(sizeof(struct Dealer), CONTEXT_DEALER);

        dealer->cards = NULL;
        dealer->player_game = NULL;

        deck_init();

        // dealer draw first 2 cards
        draw_card(deck_draw_card());
        draw_card(deck_draw_card());

        init_players();
    }
}

void dealer_play()
{
    struct PlayerGame *pivot;

    pivot = dealer->player_game;

    // player play
    while (pivot != NULL)
    {
        pivot = pivot->next;
    }

    // dealer play
    while (drawn_card_total_score(dealer->cards) < MIN_SCORE_DEALER_STOP)
    {
        draw_card(deck_draw_card());
    }

    drawn_card_print(dealer->cards);
    printf("Total dealer sccore: %u\n", drawn_card_total_score(dealer->cards));

    deck_dealloc();
    dealloc();
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

static void add_player(struct PlayerGame **head, struct PlayerGame *player_game)
{
    struct PlayerGame **pivot;

    pivot = head;

    while (*pivot != NULL)
    {
        pivot = &(*pivot)->next;
    }

    *pivot = player_game;

    // player draw first 2 cards
    player_draw_card(player_game->player, deck_draw_card());
    player_draw_card(player_game->player, deck_draw_card());
}

static void dealloc()
{
    dealloc_drawn_cards();
    dealloc_player_game();
    mfree(dealer, CONTEXT_DEALER);
}

static void dealloc_drawn_cards()
{
    struct DrawnCard *pivot, *old_node;

    pivot = dealer->cards;
    while (pivot != NULL)
    {
        old_node = pivot;
        pivot = pivot->next;
        drawn_card_dealloc(old_node);
    }
}

static void dealloc_player_game()
{
    struct PlayerGame *pivot, *old_node;

    pivot = dealer->player_game;
    while (pivot != NULL)
    {
        old_node = pivot;
        pivot = pivot->next;
        player_game_dealloc(old_node);
    }
}

static void draw_card(struct Card *card)
{
    drawn_card_push(&dealer->cards, card);
}

static void init_players()
{
    char *player_name = mmalloc(50 * sizeof(char), "player name");

#if !DEBUG
    printf("\nInsert player name: ");
    scanf("%s", player_name);
    printf("\n");
#else
    memcpy(player_name, "John", 4);
#endif

    dealer_add_player_game(player_game_init(player_init_with_name(player_name), false));
}