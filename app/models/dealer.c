#include "dealer.h"

#include <stdio.h>
#include <string.h>

#include "drawn_card.h"
#include "deck.h"
#include "player_game.h"

#include "../../libs/mmalloc/alloc/mmalloc.h"
#include "../../utils/utils.h"

#define CONTEXT_DEALER "dealer"

#define DEALER_DRAW_INITIAL_CARS(dealer, deck)      \
    dealer_draw_card(dealer, deck_draw_card(deck)); \
    dealer_draw_card(dealer, deck_draw_card(deck));

#define PLAYER_DRAW_INITIAL_CARDS(player, deck)     \
    player_draw_card(player, deck_draw_card(deck)); \
    player_draw_card(player, deck_draw_card(deck));

#ifdef DEBUG
#define PLAY_DEBUG(player, deck)                            \
    if (player_total_score(player) < MIN_SCORE_DEALER_STOP) \
    {                                                       \
        player_draw_card(player, deck_draw_card(deck));     \
    }                                                       \
    else                                                    \
    {                                                       \
        stop = true;                                        \
    }
#else
#define DOUBLE_DOWN(player, deck)                           \
    if (player_total_cards(player) <= MAX_CARDS_FOR_DOUBLE) \
    {                                                       \
        player_draw_card(player, deck_draw_card(deck));     \
        stop = true;                                        \
    }
#endif

struct Dealer
{
    DECK deck;
    DRAWN_CARD cards;
    PLAYER_GAME player_game;
};

#ifndef DEBUG
enum options
{
    card = 1,
    stand,
    double_down
};
#endif

static bool dealer_calc_final_results(const DEALER, const PLAYER_GAME);
static void dealer_dealloc_deck(DEALER);
static void dealer_dealloc_drawn_cards(DEALER);
static void dealer_dealloc_player_game(DEALER);
static void dealer_draw_card(DEALER, CARD);
static void dealer_final(DEALER);
static void dealer_init_players(DEALER);
static void dealer_init_player_game_bets(DEALER);
static void dealer_play_players(DEALER);
static void dealer_play_player_game(DEALER, PLAYER_GAME);
static void dealer_play_self(DEALER);
#ifndef DEBUG
static int dealer_show_options(const PLAYER_GAME player_game);
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
}

void dealer_dealloc(DEALER dealer)
{
    dealer_dealloc_drawn_cards(dealer);
    dealer_dealloc_player_game(dealer);
    mfree(dealer, CONTEXT_DEALER);
}

DEALER dealer_init()
{

    DEALER dealer = mmalloc(sizeof(Dealer), CONTEXT_DEALER);

    dealer->cards = NULL;
    dealer->player_game = NULL;

    dealer_init_players(dealer);

    return dealer;
}

void dealer_play(DEALER dealer)
{
    bool continue_play = true;
#ifndef DEBUG
    char continue_chooice;
#endif
    do
    {
        dealer->deck = deck_init();
        dealer_init_player_game_bets(dealer);

        DEALER_DRAW_INITIAL_CARS(dealer, dealer->deck);

        // players play
        dealer_play_players(dealer);

        // dealer play
        printf("It's dealer turns\n");
        dealer_play_self(dealer);
        drawn_card_print(dealer->cards);
        printf("Total dealer score: %u\n", drawn_card_total_score(dealer->cards));

        // calc final results and dealloc players drawn card
        dealer_final(dealer);

        dealer_dealloc_drawn_cards(dealer);
        dealer_dealloc_deck(dealer);

#ifdef DEBUG
        continue_play = false;
#else
        BREAK_LINE;
        printf("Another match?(y/n): ");
        scanf(" %c", &continue_chooice);
        getchar();
        BREAK_LINE;
        if (continue_chooice == 'n')
        {
            continue_play = false;
        }

        clear_screen();
#endif
    } while (continue_play);
}

void dealer_print_game(DEALER dealer, PLAYER_GAME player_game, printGameOptions options)
{
    char *format;
    switch (options)
    {
    case actual:
        format = "Actual player score: %u\n";
        break;
    case total:
    default:
        format = "Total player score: %u\n";
        break;
    }
    clear_screen();
    dealer_print_initial_cards(dealer);
    BREAK_LINE;
    player_print_cards(player_game->player);
    printf(format, player_total_score(player_game->player));
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

static bool dealer_calc_final_results(const DEALER dealer, const PLAYER_GAME player_game)
{
    bool win = false;

    if (player_total_score(player_game->player) > MAX_VALID_SCORE)
    {
        printf("%s loses %u$, exceeds 21\n", player_name(player_game->player), player_game->amount_bet);
    }
    else if (drawn_card_total_score(dealer->cards) > MAX_VALID_SCORE)
    {
        win = true;
        printf("%s wins %u$, dealer exceeds 21\n", player_name(player_game->player), player_game->amount_bet);
    }
    else if (player_total_score(player_game->player) <= drawn_card_total_score(dealer->cards))
    {
        printf("%s loses %u$, dealer wins\n", player_name(player_game->player), player_game->amount_bet);
    }
    else
    {
        win = true;
        printf("%s wins %u$ \n", player_name(player_game->player), player_game->amount_bet);
    }

    return win;
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

static void dealer_draw_card(DEALER dealer, CARD card)
{
    drawn_card_push(&dealer->cards, card);
}

static void dealer_final(DEALER dealer)
{
    PLAYER_GAME pivot;

    pivot = dealer->player_game;

    while (pivot != NULL)
    {
        if (dealer_calc_final_results(dealer, pivot))
        {
            player_win_amount(pivot->player, pivot->amount_bet * 2);
        }

        player_dealloc_drawn_cards(pivot->player);

        pivot = pivot->next;
    }
}

static void dealer_init_players(DEALER dealer)
{
    // for now inits one player
    char *player_name = mmalloc(50 * sizeof(char), "player name");

#ifdef DEBUG
    char *tmpName = "John";
    memcpy(player_name, tmpName, strlen(tmpName) + 1);
#else
    printf("\nInsert player name: ");
    scanf("%s", player_name);
    BREAK_LINE;
#endif

    PLAYER player = player_init_with_name(player_name);
    PLAYER_GAME player_game = player_game_init(player, false);
    dealer_add_player_game(dealer, player_game);
}

static void dealer_init_player_game_bets(DEALER dealer)
{
    PLAYER_GAME pivot;
    unsigned int amount_bet = 0;

    pivot = dealer->player_game;

    while (pivot != NULL)
    {
#ifdef DEBUG
        amount_bet = 10;
#else
        bool valid = false;
        do
        {
            printf("\nInsert amount bet: ");
            scanf("%u", &amount_bet);

            if (player_can_bet(pivot->player, amount_bet))
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
        player_bet_amount(pivot->player, amount_bet);
        pivot->amount_bet = amount_bet;

        pivot = pivot->next;
    }
}

static void dealer_play_players(DEALER dealer)
{
    PLAYER_GAME pivot;

    pivot = dealer->player_game;

    while (pivot != NULL)
    {
        dealer_play_player_game(dealer, pivot);
        dealer_print_game(dealer, pivot, total);

        pivot = pivot->next;
    }
}

static void dealer_play_player_game(DEALER dealer, PLAYER_GAME player_game)
{
    bool stop = false;

    printf("%s it's your turn!\n", player_name(player_game->player));

    PLAYER_DRAW_INITIAL_CARDS(player_game->player, dealer->deck);

    while (!stop && player_total_score(player_game->player) < MAX_VALID_SCORE)
    {
        dealer_print_game(dealer, player_game, actual);
#ifdef DEBUG
        PLAY_DEBUG(player_game->player, dealer->deck);
#else
        int s;
        s = dealer_show_options(player_game);
        switch (s)
        {
        case card:
            player_draw_card(player_game->player, deck_draw_card(dealer->deck));
            break;
        case double_down:
            DOUBLE_DOWN(player_game->player, dealer->deck);
            break;
        case stand:
            stop = true;
            break;
        default:
            break;
        }
#endif
    }
}

static void dealer_play_self(DEALER dealer)
{
    while (drawn_card_total_score(dealer->cards) < MIN_SCORE_DEALER_STOP)
    {
        dealer_draw_card(dealer, deck_draw_card(dealer->deck));
    }
}

#ifndef DEBUG
static int dealer_show_options(const PLAYER_GAME player_game)
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