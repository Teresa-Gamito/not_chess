#pragma once

#include <SDL3/SDL_stdinc.h>
#include "../../helper_functions/error_handling.h"

typedef struct Piece Piece;

typedef enum PieceType
{
    PAWN,
    BISHOP,
    KNIGHT,
    ROOK,
    QUEEN,
    KING
} PieceType;

typedef enum PieceColor
{
    PIECE_BLACK,
    PIECE_WHITE
} PieceColor;



// ========== create ==========
Piece* piece_create(PieceType type, PieceColor color);



// ========== destroy ==========
void piece_destroy(Piece *piece);



// ========== set ==========
void piece_set_type(Piece* piece, PieceType type);
void piece_set_color(Piece* piece, PieceColor color);



// ========== get ==========
PieceType piece_get_type(const Piece* piece); 
PieceColor piece_get_color(const Piece* piece);
