#pragma once

#include <SDL3/SDL_stdinc.h>
#include "include/game/board/color.h"
#include "include/helper_functions/error_handling.h"

typedef struct Piece Piece;

typedef enum PieceType
{
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING,
    LANCE,
    PIECE_TYPE_COUNT
} PieceType;

typedef enum PiecePoints
{
    POINTS_PAWN = 1,
    POINTS_ROOK = 3,
    POINTS_KNIGHT = 2,
    POINTS_BISHOP = 2,
    POINTS_QUEEN = 5,
    POINTS_KING = 0,
    POINTS_LANCE = 3,
} PiecePoints;

// ========== create ==========
Piece* piece_create(PieceType type, Color color);

// ========== destroy ==========
void piece_destroy(Piece *piece);

// ========== set ==========
void piece_set_type(Piece* piece, PieceType type);
void piece_set_color(Piece* piece, Color color);
void piece_set_moved(Piece* piece);

// ========== get ==========
PieceType piece_get_type(const Piece* piece); 
Color piece_get_color(const Piece* piece);
int piece_get_points(const Piece* piece);
bool piece_has_moved(const Piece* piece);
bool piece_can_move_to(const Piece* piece, int src_col, int src_row, int dst_col, int dst_row);
bool piece_requires_clear_path(const Piece* piece);
bool piece_can_capture(
    const Piece* src_piece, 
    const Piece* dst_piece, 
    int src_col, 
    int src_row, 
    int dst_col, 
    int dst_row
);
