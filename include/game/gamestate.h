#pragma once

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_render.h>
#include "../ui_elements/window.h"
#include "board/board.h"

#define PATH_TEXTURE_WINDOW_BACKGROUND      "assets/sprites/tile_black.png"
#define PATH_TEXTURE_BUTTON                 "assets/sprites/tile_white.png"

typedef enum ShopTextures
{
    TEXTURE_WINDOW_BACKGROUND,
    TEXTURE_BUTTON,

    SHOP_TEXTURE_COUNT
} ShopTextures;

static const char* shop_textures[] =
{
    PATH_TEXTURE_WINDOW_BACKGROUND,
    PATH_TEXTURE_BUTTON
};

typedef struct GameState GameState;

// ========== create ==========
GameState* gamestate_create();
void gamestate_set_default(SDL_Renderer* renderer, GameState* game);

// ========== destroy ==========
void gamestate_destroy(GameState* game);



// ========== render ==========
void game_render(SDL_Renderer* renderer, GameState* game);

// ========== update ==========
void gamestate_update(InputState* input, GameState* game);


// ========== set ==========
void gamestate_add_window(GameState* game, Window* window);

// ========== get ==========
Board* gamestate_get_board(GameState* game);
