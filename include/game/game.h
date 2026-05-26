#pragma once

#include <SDL3/SDL_stdinc.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "game/board/board.h"
#include "game/player/player.h"
#include "game/upgrade_tree/tree.h"
#include "game/log/log.h"
#include "data_structures/queue.h"
#include "game/rules/rules.h"
#include "helper_functions/error_handling.h"
#include "helper_functions/global_variables.h"

#define PLAYER_STARTING_POINTS 0

typedef struct Game Game;

Game* game_create();

void game_destroy(Game* game);

void game_start(Game* game);
void game_advance_turn(Game* game);
bool game_purchase_upgrade(Game* game, int index);

bool game_try_upgrade(Game* game, Pos pos);
bool game_try_capture(Game* game, Pos src, Pos dst);
bool game_try_move(Game* game, Pos src, Pos dst);

Board* game_get_board(const Game* game);
Tree* game_get_tree(const Game* game);
RuleList* game_get_rules(const Game* game);
GameLog* game_get_log(const Game* game);
Queue* game_get_upgrade_queue(const Game* game);

Player* game_get_active_player(const Game* game);
Player* game_get_opponent(const Game* game);

void verify_game(const Game* game);
