
#include "drawn_card.h"

struct Player;
/* subtracts the passed amount to player and returns result, false if no available amount */
bool player_bet_amount(struct Player *player, unsigned int amount);

void player_draw_card(struct Player *player, struct Card *card);

struct Player *player_init_with_name(char *name);

unsigned int player_stand(struct Player *player);

void player_win_amount(struct Player *player, unsigned int amount);