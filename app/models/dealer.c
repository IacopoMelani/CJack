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

static void add_player(struct Dealer **dealer, struct PlayerGame *player_game);
static void draw_card(struct Card *card);
static void init_players();

static void add_player(struct Dealer **dealer, struct PlayerGame *player_game)
{
    struct PlayerGame *pivot;

    pivot = (*dealer)->player_game;

    while (pivot != NULL)
    {
        pivot = pivot->next;
    }

    pivot = player_game;
}

void dealer_add_player_game(struct PlayerGame *player_game)
{
    add_player(&dealer, player_game);
}

void dealer_init()
{
    if (dealer == NULL)
    {
        dealer = mmalloc(sizeof(struct Dealer), CONTEXT_DEALER);

        deck_init();

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
        printf("%s", player_name(pivot->player));
        pivot = pivot->next;
    }

    return;

    // dealer play
    while (drawn_card_total_score(dealer->cards) < MIN_SCORE_DEALER_STOP)
    {
        draw_card(deck_draw_card());
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

static void draw_card(struct Card *card)
{
    drawn_card_push(dealer->cards, card);
}

static void init_players()
{
    char *player_name = mmalloc(50 * sizeof(char), "player name");

    printf("Insert player name: ");
    scanf("%s", player_name);

    dealer_add_player_game(player_game_init(player_init_with_name(player_name), false));
}