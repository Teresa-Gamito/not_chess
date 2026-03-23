#pragma once

#include <SDL3/SDL_stdinc.h>
#include "../window.h"

typedef struct GameState GameState;
typedef struct UIState UIState;

GameState* game_state_create();
void game_state_destroy(GameState* game_state);
