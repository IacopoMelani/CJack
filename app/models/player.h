
#include "drawn_card.h"

typedef struct Player Player;

typedef Player *PLAYER;

/* subtracts the passed amount to player and returns result, false if no available amount */
bool player_bet_amount(PLAYER player, unsigned int amount);

bool player_can_bet(PLAYER player, unsigned int amount);

void player_dealloc(PLAYER player);

void player_dealloc_drawn_cards(PLAYER player);

void player_draw_card(PLAYER player, CARD card);

PLAYER player_init_with_name(char *name);

char *player_name(PLAYER player);

void player_print_cards(PLAYER player);

unsigned int player_stand(PLAYER player);

unsigned int player_total_cards(PLAYER player);

unsigned int player_total_score(PLAYER player);

void player_win_amount(PLAYER player, unsigned int amount);
