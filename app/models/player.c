#include "player.h"

#include <stdio.h>
#include <string.h>

#include "../../libs/mmalloc/alloc/mmalloc.h"

#define INITIAL_BANK_ACCOUNT 20

#define CONTEXT_PLAYER "player"

struct Player
{
    char *name;
    struct DrawnCard *cards;
    unsigned int bank_account;
};

static void player_sprintf(char *buf, struct Player *player);

bool player_bet_amount(struct Player *player, unsigned int amount)
{
    if (player->bank_account < amount)
    {
        return false;
    }

    player->bank_account -= amount;

    return true;
}

void player_dealloc(struct Player *player)
{
    struct DrawnCard *pivot, *old_node;

    pivot = player->cards;
    while (pivot != NULL)
    {
        old_node = pivot;
        pivot = pivot->next;
        drawn_card_dealloc(old_node);
    }
    char buf[80];
    player_sprintf(buf, player);
    mfree(player->name, "player name");
    mfree(player, buf);
}

void player_draw_card(struct Player *player, struct Card *card)
{
    drawn_card_push(&player->cards, card);
}

struct Player *player_init_with_name(char *name)
{
    struct Player *player = mmalloc(sizeof(struct Player), CONTEXT_PLAYER);

    player->bank_account = INITIAL_BANK_ACCOUNT;
    player->cards = NULL;
    player->name = name;

    return player;
}

char *player_name(struct Player *player)
{
    return player->name;
}

unsigned int player_stand(struct Player *player)
{
    return drawn_card_total_score(player->cards);
}

void player_win_amount(struct Player *player, unsigned int amount)
{
    player->bank_account += amount;
}

static void player_sprintf(char *buf, struct Player *player)
{
    sprintf(buf, "Player name: %s, bank account: %u", player->name, player->bank_account);
}