#pragma once

#include <SDL3/SDL_stdinc.h>
#include <stdbool.h>
#include <stdbool.h>
#include "../../helper_functions/helper_functions.h"
#include "../../helper_functions/error_handling.h"
#include "../../ui_elements/window.h"
#include "piece.h"
#include "tile.h"

#define BOARD_STARTING_COL_NUM 8
#define BOARD_STARTING_ROW_NUM 8

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

typedef struct Board Board;


Board* board_create(
    SDL_Renderer* renderer, 
    int col_num, 
    int row_num, 
    double window_x, 
    double window_y, 
    double window_width, 
    double window_height
);
void board_destroy(Board* board);
void board_render(SDL_Renderer* renderer, const Board* board);
void board_update(InputState* input, Board* board);

void board_set_default_layout(Board* board);

    // board actions
void board_move_piece_from_to(Board* board, int src_col, int src_row, int dst_col, int dst_row);

void board_add_piece_at(Board* board, Piece* piece, int col, int row);

void board_change_piece(Board* board, Piece* old_piece, Piece* new_piece);
void board_change_piece_at(Board* board, int col, int row, Piece* new_piece);
void board_change_tile(Board* board, Tile* old_tile, Tile* new_tile);
void board_change_tile_at(Board* board, int col, int row, Tile* new_tile);

void board_expand(Board* board, int expand_by);
// maybe not?
void board_add_row_left(Board* board, int expand_by);
void board_add_row_right(Board* board, int expand_by);
void board_add_col_left(Board* board, int expand_by);
void board_add_col_right(Board* board, int expand_by);


    // get
int board_get_col_num(const Board* board);
int board_get_row_num(const Board* board);
Window* board_get_window(const Board* board);

// // abstract
// Window* board_window_create(SDL_Renderer* renderer, Board* board, double screen_x, double screen_y, double width, double height);
// 
// 
// void board_add_tile(Board* board, Tile* tile, int col, int row);
// 
// void board_remove_tile(Board* board, int col, int row);
// void board_remove_piece(Board* board, int col, int row);
// 
// 
// Piece* board_get_piece_at(Board* board, int col, int row);
// Tile* board_get_tile_at(Board* board, int col, int row);
// 
// void board_tile_set_object(Board* board, int col, int row);
// void board_piece_set_object(Board* board, int col, int row);
// 
// Object* board_tile_get_object(Board* board, int col, int row);
// Object* board_piece_get_object(Board* board, int col, int row);








