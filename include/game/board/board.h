#pragma once

#include <SDL3/SDL_stdinc.h>
#include <stdbool.h>
#include "piece.h"
#include "tile.h"

#define BOARD_STARTING_WIDTH 8
#define BOARD_STARTING_HEIGHT 8

typedef struct BoardState BoardState;



// ========== create ==========
BoardState* board_create();
void board_set_starting_layout(BoardState* board);



// ========== destroy ==========
void board_destroy(BoardState* board);



// ========== get ==========
int board_get_width(BoardState* board);
int board_get_height(BoardState* board);


// ========== pieces ==========
void board_add_piece_at(BoardState* board, Piece* piece, int x, int y);
void board_destroy_piece_at(BoardState* board, int x, int y);
Piece* board_get_piece_at(BoardState* board, int x, int y);
void board_move_piece(BoardState* board, int src_x, int src_y, int dst_x, int dst_y);
bool board_can_move_piece(BoardState* board, int src_x, int src_y, int dst_x, int dst_y);
bool board_can_capture_piece(Piece* piece1, Piece* piece2);


// ========== tiles ==========
void board_increment_size(BoardState* board);

void board_render(BoardState* board);
