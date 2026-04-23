#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include "include/ui_elements/window.h"
#include "include/game/board/board.h"
#include "include/helper_functions/error_handling.h"
#include "include/helper_functions/helper_functions.h"
#include "include/helper_functions/vector.h"
#include "include/ui_elements/button.h"

#define TEXTURE_DEFAULT_SIZE_PX 16

#define PATH_TEXTURE_WHITE_PAWN                 "assets/sprites/white_pawn.png"
#define PATH_TEXTURE_WHITE_ROOK                 "assets/sprites/white_rook.png"
#define PATH_TEXTURE_WHITE_KNIGHT               "assets/sprites/white_knight.png"
#define PATH_TEXTURE_WHITE_BISHOP               "assets/sprites/white_bishop.png"
#define PATH_TEXTURE_WHITE_QUEEN                "assets/sprites/white_queen.png"
#define PATH_TEXTURE_WHITE_KING                 "assets/sprites/white_king.png"

#define PATH_TEXTURE_BLACK_PAWN                 "assets/sprites/black_pawn.png"  
#define PATH_TEXTURE_BLACK_ROOK                 "assets/sprites/black_rook.png"  
#define PATH_TEXTURE_BLACK_KNIGHT               "assets/sprites/black_knight.png"
#define PATH_TEXTURE_BLACK_BISHOP               "assets/sprites/black_bishop.png"
#define PATH_TEXTURE_BLACK_QUEEN                "assets/sprites/black_queen.png" 
#define PATH_TEXTURE_BLACK_KING                 "assets/sprites/black_king.png"  

#define PATH_TEXTURE_TILE_WHITE                 "assets/sprites/tile_white.png"
#define PATH_TEXTURE_TILE_BLACK                 "assets/sprites/tile_black.png"

#define PATH_TEXTURE_TILE_SELECTED              "assets/sprites/tile_selected.png"
#define PATH_TEXTURE_TILE_VALID                 "assets/sprites/tile_valid.png"
#define PATH_TEXTURE_TILE_CAPTURE               "assets/sprites/tile_capture.png"

#define PATH_TEXTURE_WINDOW_BOARD_BACKGROUND    "assets/sprites/tile_black.png"

typedef struct BoardUI BoardUI;

typedef enum Task
{
    TASK_ADD_PAWN,
} Task;

typedef enum BoardTextures
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

    TEXTURE_TILE_WHITE,
    TEXTURE_TILE_BLACK,

    TEXTURE_TILE_SELECTED,
    TEXTURE_TILE_VALID,
    TEXTURE_TILE_CAPTURE,

    BOARD_TEXTURE_COUNT
} BoardTextures;

static const char* board_textures[] =
{
    PATH_TEXTURE_WHITE_PAWN,
    PATH_TEXTURE_WHITE_ROOK,
    PATH_TEXTURE_WHITE_KNIGHT,
    PATH_TEXTURE_WHITE_BISHOP,
    PATH_TEXTURE_WHITE_QUEEN,
    PATH_TEXTURE_WHITE_KING,

    PATH_TEXTURE_BLACK_PAWN,
    PATH_TEXTURE_BLACK_ROOK,
    PATH_TEXTURE_BLACK_KNIGHT,
    PATH_TEXTURE_BLACK_BISHOP,
    PATH_TEXTURE_BLACK_QUEEN,
    PATH_TEXTURE_BLACK_KING,

    PATH_TEXTURE_TILE_WHITE,
    PATH_TEXTURE_TILE_BLACK,

    PATH_TEXTURE_TILE_SELECTED,
    PATH_TEXTURE_TILE_VALID,
    PATH_TEXTURE_TILE_CAPTURE
};

BoardUI* board_ui_create(
    SDL_Renderer* renderer, 
    Board* board, 
    float x, 
    float y, 
    float width, 
    float height
);
void board_ui_destroy(BoardUI* ui);
void board_ui_update(BoardUI* ui);

void select_tile(void* board_ui, void* tile);
void deselect_tile(void* board_ui, void* x);

bool board_ui_has_task(BoardUI* ui);
void board_ui_task_complete_first(BoardUI* ui);
Task board_ui_get_task_first(BoardUI* ui);
void board_ui_add_task(BoardUI* ui, Task task);

Window* board_ui_get_window(BoardUI* ui);
