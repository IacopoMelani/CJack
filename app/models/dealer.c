#include "dealer.h"

#include <stdio.h>
#include <string.h>

#include "drawn_card.h"
#include "deck.h"
#include "player_game.h"

#include "../../libs/mmalloc/alloc/mmalloc.h"
#include "../utils/utils.h"

#define CONTEXT_DEALER "dealer"

struct Dealer
{
    DECK deck;
    DRAWN_CARD cards;
    PLAYER_GAME player_game;
};

static bool dealer_calc_final_results(DEALER, PLAYER_GAME);
static void dealer_dealloc_drawn_cards(DEALER);
static void dealer_dealloc_player_game(DEALER);
static void dealer_draw_card(DEALER, CARD);
static void dealer_init_players(DEALER);
#ifndef DEBUG
static int dealer_show_options(PLAYER_GAME player_game);

enum options
{
    card = 1,
    stand,
    double_down
};
#else
#define PLAY_DEBUG                                                     \
    if (player_total_score(pivot->player) < MIN_SCORE_DEALER_STOP)     \
    {                                                                  \
        player_draw_card(pivot->player, deck_draw_card(dealer->deck)); \
    }                                                                  \
    else                                                               \
    {                                                                  \
        stop = true;                                                   \
    }
#endif

void dealer_add_player_game(DEALER dealer, PLAYER_GAME player_game)
{
    PLAYER_GAME *pivot;

    pivot = &dealer->player_game;

    while (*pivot != NULL)
    {
        pivot = &(*pivot)->next;
    }

    *pivot = player_game;

    // player draw first 2 cards
    player_draw_card(player_game->player, deck_draw_card(dealer->deck));
    player_draw_card(player_game->player, deck_draw_card(dealer->deck));
}

void dealer_dealloc(DEALER dealer)
{
    deck_dealloc(dealer->deck);
    dealer_dealloc_drawn_cards(dealer);
    dealer_dealloc_player_game(dealer);
    mfree(dealer, CONTEXT_DEALER);
}

DEALER dealer_init()
{

    DEALER dealer = mmalloc(sizeof(struct Dealer), CONTEXT_DEALER);

    dealer->cards = NULL;
    dealer->player_game = NULL;

    dealer->deck = deck_init();

    // dealer draw first 2 cards
    dealer_draw_card(dealer, deck_draw_card(dealer->deck));
    dealer_draw_card(dealer, deck_draw_card(dealer->deck));

    dealer_init_players(dealer);

    return dealer;
}

void dealer_play(DEALER dealer)
{
    PLAYER_GAME pivot;

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
#ifdef DEBUG
            PLAY_DEBUG
#else
            int s;
            s = dealer_show_options(pivot);
            switch (s)
            {
            case card:
                player_draw_card(pivot->player, deck_draw_card(dealer->deck));
                break;
            case double_down:
                if (player_total_cards(pivot->player) <= MAX_CARDS_FOR_DOUBLE)
                {
                    player_draw_card(pivot->player, deck_draw_card(dealer->deck));
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
        dealer_draw_card(dealer, deck_draw_card(dealer->deck));
    }

    drawn_card_print(dealer->cards);
    printf("Total dealer score: %u\n", drawn_card_total_score(dealer->cards));

    pivot = dealer->player_game;

    while (pivot != NULL)
    {
        if (dealer_calc_final_results(dealer, pivot))
        {
            player_win_amount(pivot->player, pivot->amount_bet * 2);
        }

        pivot = pivot->next;
    }
}

void dealer_print_initial_cards(DEALER dealer)
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

static bool dealer_calc_final_results(DEALER dealer, PLAYER_GAME player_game)
{
    bool win = false;

    if (player_total_score(player_game->player) > MAX_VALID_SCORE)
    {
        printf("%s loses, exceeds 21\n", player_name(player_game->player));
    }
    else if (drawn_card_total_score(dealer->cards) > MAX_VALID_SCORE)
    {
        win = true;
        printf("%s wins, dealer exceeds 21\n", player_name(player_game->player));
    }
    else if (player_total_score(player_game->player) <= drawn_card_total_score(dealer->cards))
    {
        printf("%s loses, dealer wins\n", player_name(player_game->player));
    }
    else
    {
        win = true;
        printf("%s wins\n", player_name(player_game->player));
    }

    return win;
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

static void dealer_draw_card(DEALER dealer, CARD card)
{
    drawn_card_push(&dealer->cards, card);
}

static void dealer_init_players(DEALER dealer)
{
    char *player_name = mmalloc(50 * sizeof(char), "player name");

#ifdef DEBUG
    char *tmpName = "John";
    memcpy(player_name, tmpName, strlen(tmpName) + 1);
#else
    printf("\nInsert player name: ");
    scanf("%s", player_name);
    printf("\n");
#endif

    PLAYER player = player_init_with_name(player_name);
    unsigned int amount_bet;

#ifdef DEBUG
    amount_bet = 10;
#else
    bool valid = false;
    do
    {
        printf("\nInsert amount bet: ");
        scanf("%u", &amount_bet);

        if (player_can_bet(player, amount_bet))
        {
            valid = true;
        }
        else
        {
            clear_screen();
            printf("\nCan't bet, you don't have the money to bet");
        }

    } while (!valid);
#endif

    player_bet_amount(player, amount_bet);

    PLAYER_GAME player_game = player_game_init(player, false);
    player_game->amount_bet = 10;

    dealer_add_player_game(dealer, player_game);
}

#ifndef DEBUG
static int dealer_show_options(PLAYER_GAME player_game)
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