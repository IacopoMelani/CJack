#include "cli_game.h"

#include <stdio.h>
#include <string.h>

#include "../app/models/deck.h"

#include "../app/models/dealer.h"
#include "../libs/mmalloc/alloc/mmalloc.h"
#include "../utils/utils.h"

#ifdef DEBUG
#ifdef AUTO
#define AUTO_DEBUG
#endif
#endif

#define DEALER_DRAW_INITIAL_CARD(cli_game) \
    cli_game_dealer_draw_card(cli_game);   \
    cli_game_dealer_draw_card(cli_game);

#define PLAYER_DRAW_INITIAL_CARD(player, dealer)                       \
    player_draw_card(player, deck_draw_card(dealer_get_deck(dealer))); \
    player_draw_card(player, deck_draw_card(dealer_get_deck(dealer)));

#ifdef AUTO_DEBUG
#define PLAY_DEBUG(player, dealer)                                         \
    if (player_total_score(player) < MIN_SCORE_DEALER_STOP)                \
    {                                                                      \
        player_draw_card(player, deck_draw_card(dealer_get_deck(dealer))); \
    }                                                                      \
    else                                                                   \
    {                                                                      \
        stop = TRUE;                                                       \
    }
#else
#define DOUBLE_DOWN(player_game, dealer)                                                \
    if (player_game_can_double_down(player_game))                                       \
    {                                                                                   \
        player_game_bet_amount(player_game, player_game->amount_bet);                   \
        player_draw_card(player_game->player, deck_draw_card(dealer_get_deck(dealer))); \
        stop = TRUE;                                                                    \
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

#ifndef AUTO_DEBUG
enum options
{
    card = 1,
    stand,
    double_down
};
#endif

static BOOL cli_game_calc_final_results(CLI_GAME, PLAYER_GAME);
static void cli_game_dealer_draw_card(CLI_GAME);
static void cli_game_dealer_play(CLI_GAME);
static BOOL cli_game_final(CLI_GAME);
static void cli_game_init_deck(CLI_GAME);
static void cli_game_init_players(CLI_GAME);
static void cli_game_init_player_game_bets(CLI_GAME);
static void cli_game_players_play(CLI_GAME);
static void cli_game_play_player_game(CLI_GAME, PLAYER_GAME);
static void cli_game_print_game(CLI_GAME, PLAYER_GAME, printGameOptions);
#ifndef AUTO_DEBUG
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
    BOOL continue_play = TRUE;

#ifndef AUTO_DEBUG
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
        BREAK_LINE;
        printf("It's dealer turns\n");
        cli_game_dealer_play(cli_game);

        BOOL res = cli_game_final(cli_game);

        dealer_dealloc_drawn_card_and_deck(cli_game->dealer);

        if (!res)
        {
            break;
        }

#ifdef AUTO_DEBUG
        continue_play = FALSE;
#else
        BREAK_LINE;
        printf("Another match?(y/n): ");
        scanf(" %c", &continue_chooice);
        getchar();
        BREAK_LINE;
        if (continue_chooice == 'n')
        {
            continue_play = FALSE;
        }

        clear_screen();
#endif

    } while (continue_play);
}

static BOOL cli_game_calc_final_results(CLI_GAME cli_game, PLAYER_GAME player_game)
{
    BOOL win = FALSE;

    if (player_total_score(player_game->player) > MAX_VALID_SCORE)
    {
        printf("%s loses %u$, exceeds 21\n", player_name(player_game->player), player_game->amount_bet);
    }
    else if (drawn_card_total_score(dealer_get_drawn_card(cli_game->dealer)) > MAX_VALID_SCORE)
    {
        win = TRUE;
        printf("%s wins %u$, dealer exceeds 21\n", player_name(player_game->player), player_game->amount_bet);
    }
    else if (player_total_score(player_game->player) <= drawn_card_total_score(dealer_get_drawn_card(cli_game->dealer)))
    {
        printf("%s loses %u$, dealer wins\n", player_name(player_game->player), player_game->amount_bet);
    }
    else
    {
        win = TRUE;
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

    drawn_card_print(dealer_get_drawn_card(cli_game->dealer));
    printf("Total dealer score: %u\n", drawn_card_total_score(dealer_get_drawn_card(cli_game->dealer)));
    BREAK_LINE;
}

static BOOL cli_game_final(CLI_GAME cli_game)
{
    PLAYER_GAME pivot;

    pivot = dealer_get_player_game(cli_game->dealer);

    while (pivot != NULL)
    {
        if (cli_game_calc_final_results(cli_game, pivot))
        {
            player_win_amount(pivot->player, pivot->amount_bet * 2);
        }
        player_game_reset_amount_bet(pivot);

        player_dealloc_drawn_cards(pivot->player);

        if (!pivot->is_cpu && !player_can_bet(pivot->player, 1))
        {
            printf("Match ends, you run out of money!");
            BREAK_LINE;
            return FALSE;
        }
        // TODO: if cpu and can't bet pop from player_game

        pivot = pivot->next;
    }

    return TRUE;
}

static void cli_game_init_deck(CLI_GAME cli_game)
{
    dealer_init_deck(cli_game->dealer);
}

static void cli_game_init_players(CLI_GAME cli_game)
{
    // for now inits one player
    char *player_name = mmalloc(50 * sizeof(char), "player name");

#ifdef AUTO_DEBUG
    char *tmpName = "John";
    memcpy(player_name, tmpName, strlen(tmpName) + 1);
#else
    printf("\nInsert player name: ");
    scanf("%s", player_name);
    BREAK_LINE;
#endif

    PLAYER player = player_init_with_name(player_name);
    PLAYER_GAME player_game = player_game_init(player, FALSE);
    dealer_add_player_game(cli_game->dealer, player_game);
}

static void cli_game_init_player_game_bets(CLI_GAME cli_game)
{
    PLAYER_GAME pivot;
    unsigned int amount_bet = 0;

    pivot = dealer_get_player_game(cli_game->dealer);

    while (pivot != NULL)
    {
#ifdef AUTO_DEBUG
        amount_bet = 10;
#else
        BOOL valid = FALSE;
        do
        {
            player_print_bank_account(pivot->player);
            BREAK_LINE;
            printf("\nInsert amount bet: ");
            scanf("%u", &amount_bet);

            if (player_can_bet(pivot->player, amount_bet))
            {
                valid = TRUE;
            }
            else
            {
                clear_screen();
                printf("\nCan't bet, you don't have the money to bet");
                BREAK_LINE;
            }

        } while (!valid);
#endif
        player_game_bet_amount(pivot, amount_bet);

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
    BOOL stop = FALSE;

    printf("%s it's your turn!\n", player_name(player_game->player));

    PLAYER_DRAW_INITIAL_CARD(player_game->player, cli_game->dealer);

    while (!stop && player_total_score(player_game->player) < MAX_VALID_SCORE)
    {
        cli_game_print_game(cli_game, player_game, actual);
#ifdef AUTO_DEBUG
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
            DOUBLE_DOWN(player_game, cli_game->dealer);
            break;
        case stand:
            stop = TRUE;
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

#ifndef AUTO_DEBUG
static int cli_game_show_options(const PLAYER_GAME player_game)
{
    int s;

    printf("\n1) Ask card \n");
    printf("2) Stand \n");
    if (player_game_can_double_down(player_game))
    {
        printf("3) Double\n");
    }
    printf("Make your choice: \n");
    scanf("%d", &s);

    return s;
}
#endif