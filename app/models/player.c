#include "player.h"

#include "../../libs/mmalloc/alloc/mmalloc.h"

#define INITIAL_BANK_ACCOUNT 20

struct Player
{
    char *name;
    struct DrawnCard *cards;
    unsigned int bank_account;
};

bool player_bet_amount(struct Player *player, unsigned int amount)
{
    if (player->bank_account < amount)
    {
        return false;
    }

    player->bank_account -= amount;

    return true;
}

void player_draw_card(struct Player *player, struct Card *card)
{
    drawn_card_push(player->cards, card);
}

struct Player *player_init_with_name(char *name)
{
    struct Player *player = mmalloc(sizeof(struct Player), "player");

    player->name = name;
    player->bank_account = INITIAL_BANK_ACCOUNT;
    player->cards = NULL;

    return player;
}

unsigned int player_stand(struct Player *player)
{
    return drawn_card_total_score(player->cards);
}

void player_win_amount(struct Player *player, unsigned int amount)
{
    player->bank_account += amount;
}