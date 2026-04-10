#pragma once

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_log.h>
#include <stdbool.h>
#include "../../helper_functions/helper_functions.h"
#include "../../helper_functions/error_handling.h"
#include "piece.h"
#include "tile.h"

#define BOARD_STARTING_WIDTH 8
#define BOARD_STARTING_HEIGHT 8

typedef struct Board Board;



// ========== create ==========
Board* board_create(int width, int height);
void board_set_default(Board* board);



// ========== destroy ==========
void board_destroy(Board* board);



// ========== get ==========
int board_get_width(const Board* board);
int board_get_height(const Board* board);


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

void board_render(const Board* board);
