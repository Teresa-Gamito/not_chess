#pragma once

#include <SDL3/SDL_stdinc.h>
#include "../../helper_functions/error_handling.h"
#include "../../ui_elements/object.h"

typedef struct Piece Piece;

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


bool can_knight_move_to(int src_col, int src_row, int dst_col, int dst_row);


// ========== set ==========
void piece_set_type(Piece* piece, PieceType type);
void piece_set_color(Piece* piece, PieceColor color);
void piece_set_object(Piece* piece, Object* object);



// ========== get ==========
PieceType piece_get_type(const Piece* piece); 
PieceColor piece_get_color(const Piece* piece);
Object* piece_get_object(const Piece* piece);
