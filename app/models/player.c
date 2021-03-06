#include "player.h"

#include <stdio.h>
#include <string.h>

#include "../../libs/mmalloc/alloc/mmalloc.h"

#define INITIAL_BANK_ACCOUNT 20

#define CONTEXT_PLAYER "player"

struct Player
{
    char *name;
    DRAWN_CARD cards;
    unsigned int bank_account;
};

static void player_sprintf(char *buf, const PLAYER player);

inline unsigned int player_bank_account(const PLAYER player)
{
    return player->bank_account;
}

BOOL player_bet_amount(PLAYER player, unsigned int amount)
{
    if (player->bank_account < amount)
    {
        return FALSE;
    }

    player->bank_account -= amount;

    return TRUE;
}

BOOL player_can_bet(const PLAYER player, unsigned int amount)
{
    if (player->bank_account < amount)
    {
        return FALSE;
    }

    return TRUE;
}

void player_dealloc(PLAYER player)
{
    player_dealloc_drawn_cards(player);
    char buf[80];
    player_sprintf(buf, player);
    mfree(player->name, "player name");
    mfree(player, buf);
}

void player_dealloc_drawn_cards(PLAYER player)
{
    DRAWN_CARD pivot, old_node;

    pivot = player->cards;
    while (pivot != NULL)
    {
        old_node = pivot;
        pivot = pivot->next;
        drawn_card_dealloc(old_node);
    }

    player->cards = NULL;
}

void player_draw_card(PLAYER player, CARD card)
{
    drawn_card_push(&player->cards, card);
}

PLAYER player_init_with_name(char *name)
{
    PLAYER player = mmalloc(sizeof(struct Player), CONTEXT_PLAYER);

    player->bank_account = INITIAL_BANK_ACCOUNT;
    player->cards = NULL;
    player->name = name;

    return player;
}

inline char *player_name(const PLAYER player)
{
    return player->name;
}

void player_print_bank_account(const PLAYER player)
{
    printf("Bank account: %u", player->bank_account);
}

void player_print_cards(const PLAYER player)
{
    drawn_card_print(player->cards);
}

unsigned int player_stand(const PLAYER player)
{
    return player_total_score(player);
}

unsigned int player_total_cards(const PLAYER player)
{
    return drawn_card_total_cards(player->cards);
}

unsigned int player_total_score(const PLAYER player)
{
    return drawn_card_total_score(player->cards);
}

void player_win_amount(PLAYER player, int amount)
{
    player->bank_account += amount;
}

static void player_sprintf(char *buf, const PLAYER player)
{
    sprintf(buf, "Player name: %s, bank account: %u", player->name, player->bank_account);
}