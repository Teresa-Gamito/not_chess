#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "helper_functions/color.h"
#include "helper_functions/error_handling.h"
#include "game/board/board_ui.h"
#include "game/board/board.h"
#include "game/board/player.h"
#include "game/tree/tree.h"
#include "game/tree/tree_ui.h"
#include "ui_elements/textbox.h"
#include "ui_elements/window.h"

// #define PLAYER_STARTING_POINTS 0
#define FONT_PATH "assets/arialceb.ttf"
#define FONT_SIZE 32.0f

typedef struct GameState GameState;

GameState* gamestate_create();
void gamestate_destroy(GameState* gamestate);

void game_render(SDL_Renderer* renderer, GameState* gamestate);
void game_update(InputState* input, GameState* gamestate);

void game_start(SDL_Renderer* renderer, GameState* gamestate);
