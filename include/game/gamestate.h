#pragma once

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_render.h>
#include "../ui_elements/window.h"
#include "board/board.h"

typedef struct GameState GameState;


// ========== create ==========
GameState* gamestate_create();
void gamestate_set_default(SDL_Renderer* renderer, GameState* game);

// ========== destroy ==========
void gamestate_destroy(GameState* game);



// ========== render ==========
void game_render(SDL_Renderer* renderer, GameState* game);


// ========== set ==========
void gamestate_add_window(GameState* game, Window* window);

// ========== get ==========
Board* gamestate_get_board(GameState* game);
