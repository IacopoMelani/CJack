#ifndef CLI_GAME_H
#define CLI_GAME_H

typedef struct CliGame CliGame;

typedef CliGame *CLI_GAME;

void cli_game_dealloc(CLI_GAME);

CLI_GAME cli_game_init();

void cli_game_play(CLI_GAME);

#endif