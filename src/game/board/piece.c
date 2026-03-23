#include "../../../include/game/board/piece.h"

struct Piece 
{
    PieceType type;
    PieceColour colour;
};

//struct PieceProperties

// ========== create ==========
Piece *piece_create (PieceType type, PieceColour colour) 
{
    Piece *piece = malloc(sizeof(Piece));
    piece->type = type;
    piece->colour = colour;
    return piece;
}

// ========== destroy ==========
void piece_destroy (Piece *piece) 
{
    free(piece);
}

// ========== set ==========
void piece_set_type(Piece* piece, PieceType type) 
{
    piece->type = type;
}
void piece_set_colour(Piece* piece, PieceColour colour)
{
    piece->colour = colour;
}

// ========== get ==========
PieceColour piece_get_colour(const Piece* piece)
{
    return piece->colour;
}
PieceType piece_get_type(const Piece* piece)
{
    return piece->type;
}













