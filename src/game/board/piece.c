#include "../../../include/game/board/piece.h"

struct Piece 
{
    PieceType type;
    PieceColor color;

    Object* object;
};

//struct PieceProperties



// ========== create ==========
Piece* piece_create(PieceType type, PieceColor color) 
{
    verify(type < 0 || type > PIECE_TYPE_COUNT, "Invalid PieceType");
    verify(color < 0 || color > PIECE_COLOR_COUNT, "Invalid PieceColor");

    Piece* piece = (Piece*)SDL_malloc(sizeof(Piece));
    verify(piece == NULL, "Piece could not be created: malloc");

    piece->type = type;
    piece->color = color;

    piece->object = NULL;

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
    verify(type < 0 || type > PIECE_TYPE_COUNT, "Invalid PieceType");

    piece->type = type;
}
void piece_set_colour(Piece* piece, PieceColor color)
{
    verify(piece == NULL, "Piece does not exist");
    verify(color < 0 || color > PIECE_COLOR_COUNT, "Invalid PieceColor");

    piece->color = color;
}
void piece_set_object(Piece* piece, Object* object)
{
    verify(piece == NULL, "Piece does not exist");
    verify(object == NULL, "Object does not exist");

    piece->object = object;
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
Object* piece_get_object(const Piece* piece)
{
    verify(piece == NULL, "Piece does not exist");

    return piece->object;
}
