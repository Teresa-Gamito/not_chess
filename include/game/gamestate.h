#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include "game/board/board_ui.h"
#include "game/tree/tree.h"
#include "game/tree/tree_ui.h"

// #define PLAYER_STARTING_POINTS 0

typedef struct GameState GameState;

GameState* gamestate_create();
void gamestate_destroy(GameState* gamestate);

void game_render(SDL_Renderer* renderer, GameState* gamestate);
void game_update(InputState* input, GameState* gamestate);

void game_start(SDL_Renderer* renderer, GameState* gamestate);
