#include "../../../include/game/board/piece.h"

struct Piece 
{
    PieceType type;
    PieceColor color;
};

//struct PieceProperties



// ========== create ==========
Piece* piece_create (PieceType type, PieceColor color) 
{
    Piece* piece = (Piece*)SDL_malloc(sizeof(Piece));
    piece->type = type;
    piece->color = color;
    return piece;
}



// ========== destroy ==========
void piece_destroy (Piece *piece) 
{
    SDL_free(piece);
}



// ========== set ==========
void piece_set_type(Piece* piece, PieceType type) 
{
    piece->type = type;
}
void piece_set_colour(Piece* piece, PieceColor color)
{
    piece->color = color;
}



// ========== get ==========
PieceType piece_get_type(const Piece* piece)
{
    return piece->type;
}
PieceColor piece_get_color(const Piece* piece)
{
    return piece->color;
}



bool piece_can_capture(const Piece* piece1, const Piece* piece2)
{
    return piece1->color != piece2->color;
}

bool piece_exists(const Piece* piece)
{
    return piece != NULL;
}










