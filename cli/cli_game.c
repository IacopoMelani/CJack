#include "cli_game.h"

#include <stdio.h>
#include <string.h>

#include "../app/models/deck.h"

#include "../app/models/dealer.h"
#include "../libs/mmalloc/alloc/mmalloc.h"
#include "../utils/utils.h"

#define DEALER_DRAW_INITIAL_CARD(cli_game) \
    cli_game_dealer_draw_card(cli_game);   \
    cli_game_dealer_draw_card(cli_game);

#define PLAYER_DRAW_INITIAL_CARD(player, dealer)                       \
    player_draw_card(player, deck_draw_card(dealer_get_deck(dealer))); \
    player_draw_card(player, deck_draw_card(dealer_get_deck(dealer)));

#ifdef DEBUG
#define PLAY_DEBUG(player, dealer)                                         \
    if (player_total_score(player) < MIN_SCORE_DEALER_STOP)                \
    {                                                                      \
        player_draw_card(player, deck_draw_card(dealer_get_deck(dealer))); \
    }                                                                      \
    else                                                                   \
    {                                                                      \
        stop = true;                                                       \
    }
#else
#define DOUBLE_DOWN(player, dealer)                                        \
    if (player_total_cards(player) <= MAX_CARDS_FOR_DOUBLE)                \
    {                                                                      \
        player_draw_card(player, deck_draw_card(dealer_get_deck(dealer))); \
        stop = true;                                                       \
    }
#endif

struct CliGame
{
    DEALER dealer;
};

typedef enum
{
    actual,
    total
} printGameOptions;

#ifndef DEBUG
enum options
{
    card = 1,
    stand,
    double_down
};
#endif

static bool cli_game_calc_final_results(CLI_GAME, PLAYER_GAME);
static void cli_game_dealer_draw_card(CLI_GAME);
static void cli_game_dealer_play(CLI_GAME);
static void cli_game_final(CLI_GAME);
static void cli_game_init_deck(CLI_GAME);
static void cli_game_init_players(CLI_GAME);
static void cli_game_init_player_game_bets(CLI_GAME);
static void cli_game_players_play(CLI_GAME);
static void cli_game_play_player_game(CLI_GAME, PLAYER_GAME);
static void cli_game_print_game(CLI_GAME, PLAYER_GAME, printGameOptions);
#ifndef DEBUG
static int cli_game_show_options(const PLAYER_GAME player_game);
#endif

void cli_game_dealloc(CLI_GAME cli_game)
{
    dealer_dealloc(cli_game->dealer);
    mfree(cli_game, "cli_game");
}

CLI_GAME cli_game_init()
{
    CLI_GAME cli_game = mmalloc(sizeof(struct CliGame), "cli_game");
    cli_game->dealer = dealer_init();

    cli_game_init_players(cli_game);

    return cli_game;
}

void cli_game_play(CLI_GAME cli_game)
{
    bool continue_play = true;

#ifndef DEBUG
    char continue_chooice;
#endif
    do
    {
        cli_game_init_deck(cli_game);
        cli_game_init_player_game_bets(cli_game);

        DEALER_DRAW_INITIAL_CARD(cli_game);

        // players play
        cli_game_players_play(cli_game);

        // dealer play
        printf("It's dealer turns\n");
        cli_game_dealer_play(cli_game);
        drawn_card_print(dealer_get_drawn_card(cli_game->dealer));
        printf("Total dealer score: %u\n", drawn_card_total_score(dealer_get_drawn_card(cli_game->dealer)));

        cli_game_final(cli_game);

        dealer_dealloc_drawn_card_and_deck(cli_game->dealer);

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

static bool cli_game_calc_final_results(CLI_GAME cli_game, PLAYER_GAME player_game)
{
    bool win = false;

    if (player_total_score(player_game->player) > MAX_VALID_SCORE)
    {
        printf("%s loses %u$, exceeds 21\n", player_name(player_game->player), player_game->amount_bet);
    }
    else if (drawn_card_total_score(dealer_get_drawn_card(cli_game->dealer)) > MAX_VALID_SCORE)
    {
        win = true;
        printf("%s wins %u$, dealer exceeds 21\n", player_name(player_game->player), player_game->amount_bet);
    }
    else if (player_total_score(player_game->player) <= drawn_card_total_score(dealer_get_drawn_card(cli_game->dealer)))
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

static void cli_game_dealer_draw_card(CLI_GAME cli_game)
{
    dealer_draw_card(cli_game->dealer, deck_draw_card(dealer_get_deck(cli_game->dealer)));
}

static void cli_game_dealer_play(CLI_GAME cli_game)
{
    while (drawn_card_total_score(dealer_get_drawn_card(cli_game->dealer)) < MIN_SCORE_DEALER_STOP)
    {
        dealer_draw_card(cli_game->dealer, deck_draw_card(dealer_get_deck(cli_game->dealer)));
    }
}

static void cli_game_final(CLI_GAME cli_game)
{
    PLAYER_GAME pivot;

    pivot = dealer_get_player_game(cli_game->dealer);

    while (pivot != NULL)
    {
        if (cli_game_calc_final_results(cli_game, pivot))
        {
            player_win_amount(pivot->player, pivot->amount_bet * 2);
        }

        player_dealloc_drawn_cards(pivot->player);

        pivot = pivot->next;
    }
}

static void cli_game_init_deck(CLI_GAME cli_game)
{
    dealer_init_deck(cli_game->dealer);
}

static void cli_game_init_players(CLI_GAME cli_game)
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
    dealer_add_player_game(cli_game->dealer, player_game);
}

static void cli_game_init_player_game_bets(CLI_GAME cli_game)
{
    PLAYER_GAME pivot;
    unsigned int amount_bet = 0;

    pivot = dealer_get_player_game(cli_game->dealer);

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

static void cli_game_players_play(CLI_GAME cli_game)
{
    PLAYER_GAME pivot;
    pivot = dealer_get_player_game(cli_game->dealer);

    while (pivot != NULL)
    {
        cli_game_play_player_game(cli_game, pivot);
        cli_game_print_game(cli_game, pivot, total);

        pivot = pivot->next;
    }
}

static void cli_game_play_player_game(CLI_GAME cli_game, PLAYER_GAME player_game)
{
    bool stop = false;

    printf("%s it's your turn!\n", player_name(player_game->player));

    PLAYER_DRAW_INITIAL_CARD(player_game->player, cli_game->dealer);

    while (!stop && player_total_score(player_game->player) < MAX_VALID_SCORE)
    {
        cli_game_print_game(cli_game, player_game, actual);
#ifdef DEBUG
        PLAY_DEBUG(player_game->player, cli_game->dealer);
#else
        int s;
        s = cli_game_show_options(player_game);
        switch (s)
        {
        case card:
            player_draw_card(player_game->player, deck_draw_card(dealer_get_deck(cli_game->dealer)));
            break;
        case double_down:
            DOUBLE_DOWN(player_game->player, cli_game->dealer);
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

static void cli_game_print_game(CLI_GAME cli_game, PLAYER_GAME player_game, printGameOptions options)
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
    dealer_print_initial_cards(cli_game->dealer);
    BREAK_LINE;
    player_print_cards(player_game->player);
    printf(format, player_total_score(player_game->player));
}

#ifndef DEBUG
static int cli_game_show_options(const PLAYER_GAME player_game)
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