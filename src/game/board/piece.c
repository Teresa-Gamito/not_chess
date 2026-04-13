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



bool can_move_to(int table[5][5], int src_col, int src_row, int dst_col, int dst_row)
{
    int piece_col;
    int piece_row;

    for (int col = 0; col < 5; col++)
    {
        for (int row = 0; row < 5; row++)
        {
            if (table[col][row] == 2)
            {
                piece_col = col;
                piece_row = row;
            }
        }
    }

    for (int col = 0; col < 5; col++)
    {
        for (int row = 0; row < 5; row++)
        {
            if (table[col][row] == 1 &&
                src_col - dst_col == piece_col - col &&
                src_row - dst_row == piece_row - row)
            {
                return true;
            }
        }
    }
    return false;
}
bool can_knight_move_to(int src_col, int src_row, int dst_col, int dst_row)
{
    int moves[5][5] =
    {
        {0, 1, 0, 1, 0},
        {1, 0, 0, 0, 1},
        {0, 0, 2, 0, 0},
        {1, 0, 0, 0, 1},
        {0, 1, 0, 1, 0}
    };
    if (can_move_to(moves, src_col, src_row, dst_col, dst_row))
    {
        return true;
    }
    return false;
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
