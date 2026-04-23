#pragma once

#include <SDL3/SDL_stdinc.h>
#include <stdbool.h>
#include "include/helper_functions/error_handling.h"
#include "include/helper_functions/helper_functions.h"
#include "include/game/board/piece.h"
#include "include/game/board/tile.h"

#define BOARD_STARTING_COL_NUM 8
#define BOARD_STARTING_ROW_NUM 8

typedef struct Board Board;

// ========== board ==========
Board* board_create(int col_num, int row_num);
void board_destroy(Board* board);

int board_get_col_num(const Board* board);
int board_get_row_num(const Board* board);

// ========== piece ==========
bool board_can_add_piece_at(Board* board, int col, int row);
void board_add_piece_at(Board* board, Piece* piece, int col, int row);

void board_piece_remove(Board* board, int col, int row);

bool board_has_piece_at(const Board* board, int col, int row);
Piece* board_get_piece_at(const Board* board, int col, int row);

bool board_can_piece_capture(Board* board, int src_col, int src_row, int dst_row, int dst_col);
void board_piece_capture(Board* board, int src_col, int src_row, int dst_row, int dst_col);

bool board_can_piece_move_to(Board *board, int src_col, int src_row, int dst_col, int dst_row);
void board_piece_move_to(Board* board, int src_col, int src_row, int dst_col, int dst_row);

int board_piece_get_col(const Board* board, const Piece* piece);
int board_piece_get_row(const Board* board, const Piece* piece);


// ========== tile ==========
bool board_has_tile_at(const Board* board, int col, int row);
Tile* board_get_tile_at(const Board* board, int col, int row);

int board_tile_get_col(const Board* board, const Tile* tile);
int board_tile_get_row(const Board* board, const Tile* tile);

