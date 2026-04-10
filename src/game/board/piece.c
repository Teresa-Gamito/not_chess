#include "../../../include/game/board/piece.h"

struct Piece 
{
    PieceType type;
    PieceColor color;
};

//struct PieceProperties



// ========== create ==========
Piece* piece_create(PieceType type, PieceColor color) 
{
    Piece* piece = (Piece*)SDL_malloc(sizeof(Piece));
    piece->type = type;
    piece->color = color;
    return piece;
}



// ========== destroy ==========
void piece_destroy(Piece* piece) 
{
    verify(piece == NULL, "Piece does not exist");

    SDL_free(piece);
}



// ========== set ==========
void piece_set_type(Piece* piece, PieceType type) 
{
    verify(piece == NULL, "Piece does not exist");

    piece->type = type;
}
void piece_set_colour(Piece* piece, PieceColor color)
{
    verify(piece == NULL, "Piece does not exist");

    piece->color = color;
}



// ========== get ==========
PieceType piece_get_type(const Piece* piece)
{
    verify(piece == NULL, "Piece does not exist");

    return piece->type;
}
PieceColor piece_get_color(const Piece* piece)
{
    verify(piece == NULL, "Piece does not exist");

    return piece->color;
}
