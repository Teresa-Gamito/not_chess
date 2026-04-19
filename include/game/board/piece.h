#pragma once

#include <SDL3/SDL_stdinc.h>
#include "../../helper_functions/error_handling.h"
#include "../../ui_elements/object.h"

typedef struct Piece Piece;

typedef struct PieceProperties PieceProperties;

typedef enum PieceType
{
    PAWN,
    BISHOP,
    KNIGHT,
    ROOK,
    QUEEN,
    KING,
    PIECE_TYPE_COUNT
} PieceType;

typedef enum PieceColor
{
    PIECE_BLACK,
    PIECE_WHITE,
    PIECE_COLOR_COUNT
} PieceColor;

// ========== create ==========
Piece* piece_create(PieceType type, PieceColor color);

// ========== destroy ==========
void piece_destroy(Piece *piece);

// ========== set ==========
void piece_set_type(Piece* piece, PieceType type);
void piece_set_color(Piece* piece, PieceColor color);
void piece_set_object(Piece* piece, Object* object);
void piece_set_moved(Piece* piece);

// ========== get ==========
PieceType piece_get_type(const Piece* piece); 
PieceColor piece_get_color(const Piece* piece);
bool piece_has_moved(const Piece* piece);
Object* piece_get_object(const Piece* piece);
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
