#pragma once

#include <stdlib.h>

typedef struct GameState GameState;
typedef struct UIState UIState;

GameState* game_state_create();
void game_state_destroy(GameState* game_state);
