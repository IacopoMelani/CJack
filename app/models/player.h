
#include "drawn_card.h"

typedef struct Player Player;

typedef Player *PLAYER;

unsigned int player_bank_account(const PLAYER player);

/* subtracts the passed amount to player and returns result, false if no available amount */
bool player_bet_amount(PLAYER player, unsigned int amount);

bool player_can_bet(const PLAYER player, unsigned int amount);

void player_dealloc(PLAYER player);

void player_dealloc_drawn_cards(PLAYER player);

void player_draw_card(PLAYER player, CARD card);

PLAYER player_init_with_name(char *name);

char *player_name(const PLAYER player);

void player_print_bank_account(const PLAYER player);

void player_print_cards(const PLAYER player);

unsigned int player_stand(const PLAYER player);

unsigned int player_total_cards(const PLAYER player);

unsigned int player_total_score(const PLAYER player);

void player_win_amount(PLAYER player, int amount);
