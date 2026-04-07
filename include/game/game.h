#pragma once

#include <SDL3/SDL_stdinc.h>
#include "../util/window.h"
#include "board/board.h"

typedef struct GameState GameState;

// create
GameState* game_create();

// destroy
void game_destroy(GameState* game);


// windows
void game_add_window(GameState* game, Window* window);

// render
void game_render(GameState* game);
