// responsible for processing the board and interactions between pieces and tiles
#pragma once

#include <SDL3/SDL_stdinc.h>
#include <stdbool.h>
#include "game/board/board_elements/piece.h"
#include "game/board/board_elements/tile.h"
#include "helper_functions/position.h"
#include "data_structures/vector.h"
#include "helper_functions/helper_functions.h"
#include "helper_functions/error_handling.h"

typedef struct Board Board;

// ========== create ==========
Board* board_create(int col_num, int row_num);

// ========== destroy ==========
void board_destroy(Board* board);

// ========== set ==========
void board_set_default(Board* board);

// ========== get ==========
int board_get_col_num(const Board* board);
int board_get_row_num(const Board* board);
Tile* board_get_tile_at(const Board* board, Pos pos);
Piece* board_get_piece_at(const Board* board, Pos pos);

// ========== piece ==========
void board_add_piece_at(Board* board, Piece* piece, Pos pos);

bool board_has_piece_at(const Board* board, Pos pos);
Pos board_piece_get_pos(const Board* board, const Piece* piece);

bool board_can_piece_move_to(const Board* board, Pos src, Pos dst);
// returns the points obtained
int board_piece_move_to(Board* board, Pos src, Pos dst);
bool board_can_piece_capture(const Board* board, Pos src, Pos dst);

// ========== tiles ==========
void board_expand(Board* board);
Pos board_tile_get_pos(const Board* board, const Tile* tile);

// ========== verify ==========
void verify_board_pos(const Board* board, Pos pos);
void verify_board(const Board* board);
