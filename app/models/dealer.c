#include "dealer.h"

#include <stdio.h>
#include <string.h>

#include "drawn_card.h"
#include "deck.h"
#include "player_game.h"

#include "../../libs/mmalloc/alloc/mmalloc.h"
#include "../utils/utils.h"

#ifndef DEBUG
#define DEBUG 0
#endif

#define CONTEXT_DEALER "dealer"

enum options
{
    card = 1,
    stand,
    double_down
};

struct Dealer
{
    struct DrawnCard *cards;
    struct PlayerGame *player_game;
};

static struct Dealer *dealer;

static void add_player(struct PlayerGame **head, struct PlayerGame *player_game);
static void calc_final_results(struct Dealer *dealer, struct PlayerGame *player_game);
static void dealloc();
static void dealloc_drawn_cards();
static void dealloc_player_game();
static void draw_card(struct Card *card);
static void init_players();
#if !DEBUG
static int show_options(struct PlayerGame *player_game);
#endif

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
        bool stop = false;

        printf("%s it's your turn!\n", player_name(pivot->player));

        while (!stop && player_total_score(pivot->player) < MAX_VALID_SCORE)
        {
            clear_screen();
            player_print_cards(pivot->player);
            printf("Actual player score: %u\n", player_total_score(pivot->player));
#if DEBUG
            if (player_total_score(pivot->player) < MIN_SCORE_DEALER_STOP)
            {
                player_draw_card(pivot->player, deck_draw_card());
            }
            else
            {
                stop = true;
            }
#else
            int s;
            s = show_options(pivot);
            switch (s)
            {
            case card:
                player_draw_card(pivot->player, deck_draw_card());
                break;
            case double_down:
                if (player_total_cards(pivot->player) <= MAX_CARDS_FOR_DOUBLE)
                {
                    player_draw_card(pivot->player, deck_draw_card());
                    stop = true;
                }
                break;
            case stand:
                stop = true;
                break;
            default:
                break;
            }
#endif
        }
        clear_screen();
        player_print_cards(pivot->player);
        printf("Total player score: %u\n", player_total_score(pivot->player));

        pivot = pivot->next;
    }

    printf("\nIt's dealer turns\n");

    // dealer play
    while (drawn_card_total_score(dealer->cards) < MIN_SCORE_DEALER_STOP)
    {
        draw_card(deck_draw_card());
    }

    drawn_card_print(dealer->cards);
    printf("Total dealer score: %u\n", drawn_card_total_score(dealer->cards));

    calc_final_results(dealer, dealer->player_game);

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

static void calc_final_results(struct Dealer *dealer, struct PlayerGame *player_game)
{
    if (player_total_score(player_game->player) > MAX_VALID_SCORE)
    {
        printf("%s loses, exceeds 21\n", player_name(player_game->player));
    }
    else if (drawn_card_total_score(dealer->cards) > MAX_VALID_SCORE)
    {
        printf("%s wins, dealer exceeds 21\n", player_name(player_game->player));
    }
    else if (player_total_score(player_game->player) <= drawn_card_total_score(dealer->cards))
    {
        printf("%s loses, dealer wins\n", player_name(player_game->player));
    }
    else
    {
        printf("%s wins\n", player_name(player_game->player));
    }
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

#if !DEBUG
static int show_options(struct PlayerGame *player_game)
{
    int s;

    printf("\n1) Ask card \n");
    printf("2) Stand \n");
    if (player_total_cards(player_game->player) <= MAX_CARDS_FOR_DOUBLE)
    {
        printf("3) Double\n");
    }
    printf("Make your choice: \n");
    scanf("%d", &s);

    return s;
}
#endif