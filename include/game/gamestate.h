#pragma once

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_render.h>
#include "../ui_elements/window.h"
#include "board/board.h"

// texture paths
#define PATH_TEXTURE_WHITE_PAWN     "assets/sprites/white_pawn.png"
#define PATH_TEXTURE_WHITE_ROOK     "assets/sprites/white_rook.png"
#define PATH_TEXTURE_WHITE_KNIGHT   "assets/sprites/white_knight.png"
#define PATH_TEXTURE_WHITE_BISHOP   "assets/sprites/white_bishop.png"
#define PATH_TEXTURE_WHITE_QUEEN    "assets/sprites/white_queen.png"
#define PATH_TEXTURE_WHITE_KING     "assets/sprites/white_king.png"

#define PATH_TEXTURE_BLACK_PAWN     "assets/sprites/black_pawn.png"  
#define PATH_TEXTURE_BLACK_ROOK     "assets/sprites/black_rook.png"  
#define PATH_TEXTURE_BLACK_KNIGHT   "assets/sprites/black_knight.png"
#define PATH_TEXTURE_BLACK_BISHOP   "assets/sprites/black_bishop.png"
#define PATH_TEXTURE_BLACK_QUEEN    "assets/sprites/black_queen.png" 
#define PATH_TEXTURE_BLACK_KING     "assets/sprites/black_king.png"  

#define PATH_TEXTURE_BOARD_WHITE    "assets/sprites/tile_white.png"
#define PATH_TEXTURE_BOARD_BLACK    "assets/sprites/tile_black.png"

typedef struct GameState GameState;

typedef enum GameTextures
{
    TEXTURE_WHITE_PAWN,
    TEXTURE_WHITE_ROOK,
    TEXTURE_WHITE_KNIGHT,
    TEXTURE_WHITE_BISHOP,
    TEXTURE_WHITE_QUEEN,
    TEXTURE_WHITE_KING,

    TEXTURE_BLACK_PAWN,
    TEXTURE_BLACK_ROOK,
    TEXTURE_BLACK_KNIGHT,
    TEXTURE_BLACK_BISHOP,
    TEXTURE_BLACK_QUEEN,
    TEXTURE_BLACK_KING,

    TEXTURE_BOARD_WHITE,
    TEXTURE_BOARD_BLACK,

    TEXTURE_COUNT
} GameTextures;

// ========== create ==========
GameState* gamestate_create();

// ========== destroy ==========
void gamestate_destroy(GameState* game);


void gamestate_set_default(SDL_Renderer* renderer, GameState* game);

void gamestate_load_textures(SDL_Renderer* renderer, GameState* game);
void gamestate_render(SDL_Renderer* renderer, GameState* game);


// ========== set ==========
void gamestate_add_window(GameState* game, Window* window);

// ========== get ==========
Board* gamestate_get_board(GameState* game);
