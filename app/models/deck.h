#pragma once

#include "card.h"

#define DECK_LEN 52

void deck_dealloc();

struct Card *deck_draw_card();

void deck_init();
