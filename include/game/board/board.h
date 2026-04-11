#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_log.h>
#include <stdbool.h>
#include "../../helper_functions/helper_functions.h"
#include "../../helper_functions/error_handling.h"
#include "piece.h"
#include "tile.h"
#include "../../ui_elements/window.h"

#define BOARD_STARTING_COL_NUM 8
#define BOARD_STARTING_ROW_NUM 8

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

#define PATH_TEXTURE_TILE_WHITE    "assets/sprites/tile_white.png"
#define PATH_TEXTURE_TILE_BLACK    "assets/sprites/tile_black.png"

#define PATH_TEXTURE_WINDOW_BOARD_BACKGROUND    "assets/sprites/tile_black.png"

typedef struct Board Board;

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

    BOARD_TEXTURE_COUNT
} BoardTextures;



// ========== create ==========
Board* board_create(int col_num, int row_num);
void board_set_default(Board* board);

// ========== destroy ==========
void board_destroy(Board* board);

// ========== window ==========
Window* board_create_window(
    SDL_Renderer* renderer, 
    Board* board, 
    double x, 
    double y, 
    double width, 
    double height
);

// ========== get ==========
int board_get_col_num(const Board* board);
int board_get_row_num(const Board* board);


// ========== pieces ==========
void board_add_piece_at(Board* board, Piece* piece, int x, int y);
void board_destroy_piece_at(Board* board, int x, int y);
Piece* board_get_piece_at(Board* board, int x, int y);
bool board_has_piece_at(Board* board, int x, int y);
void board_move_piece(Board* board, int src_x, int src_y, int dst_x, int dst_y);
bool board_can_move_piece(Board* board, int src_x, int src_y, int dst_x, int dst_y);
bool board_can_capture_piece(Piece* piece1, Piece* piece2);


// ========== tiles ==========
Tile* board_get_tile_at(const Board* board, int x, int y);
void board_increment_size(Board* board);

