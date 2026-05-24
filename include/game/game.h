// responsible for processing, updating and rendering the game
#pragma once

#define PLAYER_STARTING_POINTS 0

#include <SDL3/SDL_stdinc.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "game/board/board.h"
#include "game/player/player.h"
#include "game/upgrade_tree/tree.h"
#include "helper_functions/error_handling.h"
#include "helper_functions/global_variables.h"

typedef struct Game Game;

Game* game_create();
void game_destroy(Game* game);

void game_start(Game* game);

void game_advance_turn(Game* game);

Board* game_get_board(const Game* game);
Player* game_get_active_player(const Game* game);

void verify_game(const Game* game);
