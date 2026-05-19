// responsible for processing, updating and rendering the game
#pragma once

#include <SDL3/SDL_stdinc.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "helper_functions/global_variables.h"
#include "helper_functions/error_handling.h"
#include "game/board/board.h"

typedef struct Game Game;

Game* game_create();
void game_destroy(Game* game);

void game_start(Game* game);
void game_restart(Game* game);

Board* game_get_board(const Game* game);

void verify_game(const Game* game);
