#include "../../../include/game/board/piece.h"

struct Piece 
{
    PieceType type;
    PieceColor color;

    Object* object;
};

static bool can_piece_type_move_to(Piece* piece, int src_col, int src_row, int dst_col, int dst_row);
static bool can_pawn_move(int src_col, int src_row, int dst_col, int dst_row);
static bool can_rook_move(int src_col, int src_row, int dst_col, int dst_row);
static bool can_knight_move(int src_col, int src_row, int dst_col, int dst_row);
static bool can_bishop_move(int src_col, int src_row, int dst_col, int dst_row);
static bool can_queen_move(int src_col, int src_row, int dst_col, int dst_row);
static bool can_king_move(int src_col, int src_row, int dst_col, int dst_row);

static bool can_piece_type_capture(Piece* piece, int src_col, int src_row, int dst_col, int dst_row);
static bool can_pawn_capture(int src_col, int src_row, int dst_col, int dst_row);



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


// ========== movement ==========
bool piece_can_move_to(Piece* piece, int src_col, int src_row, int dst_col, int dst_row)
{
    verify(piece == NULL, "Piece does not exist");

    PieceType type = piece_get_type(piece);
    if (!can_piece_type_move_to(piece, src_col, src_row, dst_col, dst_row))
    {
        return false;
    }

    return true;
}
bool piece_requires_clear_path(Piece* piece)
{
    PieceType type = piece_get_type(piece);
    switch (type)
    {
        case PAWN:
        case ROOK:
        case BISHOP:
        case QUEEN:
            return true;

        default:
            return false;
    }
}
static bool can_piece_type_move_to(Piece* piece, int src_col, int src_row, int dst_col, int dst_row)
{
    verify(piece == NULL, "Piece does not exist");

    PieceType type = piece_get_type(piece);
    PieceColor color = piece_get_color(piece);
    switch(type)
    {
        case PAWN:
            if (color == PIECE_WHITE)
                return can_pawn_move(src_col, src_row, dst_col, dst_row);
            else if (color == PIECE_BLACK)
                return can_pawn_move(-src_col, src_row, -dst_col, dst_row);
            break;
        case ROOK:
            return can_rook_move(src_col, src_row, dst_col, dst_row);
            break;
        case KNIGHT:
            return can_knight_move(src_col, src_row, dst_col, dst_row);
            break;
        case BISHOP:
            return can_bishop_move(src_col, src_row, dst_col, dst_row);
            break;
        case QUEEN:
            return can_queen_move(src_col, src_row, dst_col, dst_row);
            break;
        case KING:
            return can_king_move(src_col, src_row, dst_col, dst_row);
        default:
            break;
    }
    return false;
}
static bool can_pawn_move(int src_col, int src_row, int dst_col, int dst_row)
{
    if (src_col == dst_col && dst_row == src_row + 1)
    {
        return true;
    }
    return false;
}
static bool can_rook_move(int src_col, int src_row, int dst_col, int dst_row)
{
    if (src_col == dst_col || src_row == dst_row)
    {
        return true;
    }
    return false;
}
static bool can_knight_move(int src_col, int src_row, int dst_col, int dst_row)
{
    if ((SDL_abs(src_col - dst_col) == 1 && SDL_abs(src_row - dst_row) == 2) ||
        (SDL_abs(src_col - dst_col) == 2 && SDL_abs(src_row - dst_row) == 1))
    {
        return true;
    }
    return false;
}
static bool can_bishop_move(int src_col, int src_row, int dst_col, int dst_row)
{
    if (dst_col - src_col == dst_row - src_row || dst_row - src_row == -dst_col + src_col)
    {
        return true;
    }
    return false;
}
static bool can_queen_move(int src_col, int src_row, int dst_col, int dst_row)
{
    if (dst_col - src_col == dst_row - src_row || dst_row - src_row == -dst_col + src_col ||
        src_col == dst_col || src_row == dst_row)
    {
        return true;
    }
    return false;
}
static bool can_king_move(int src_col, int src_row, int dst_col, int dst_row)
{
    if ((SDL_abs(src_col - dst_col) == 1 && SDL_abs(src_row - dst_row) == 1) ||
        (SDL_abs(src_col - dst_col) == 0 && SDL_abs(src_row - dst_row) == 1) ||
        (SDL_abs(src_col - dst_col) == 1 && SDL_abs(src_row - dst_row) == 0))
    {
        return true;
    }
    return false;
}


bool piece_can_capture(Piece* src_piece, Piece* dst_piece, int src_col, int src_row, int dst_col, int dst_row)
{
    verify(src_piece == NULL, "Piece does not exist");
    verify(dst_piece == NULL, "Piece does not exist");

    if (piece_get_color(src_piece) == piece_get_color(dst_piece))
    {
        return false;
    }
    if (can_piece_type_capture(src_piece, src_col, src_row, dst_col, dst_row))
    {
        return true;
    }
    return false;
}
static bool can_piece_type_capture(Piece* piece, int src_col, int src_row, int dst_col, int dst_row)
{
    verify(piece == NULL, "Piece does not exist");

    PieceType type = piece_get_type(piece);
    switch (type) 
    {
        case PAWN:
            can_pawn_capture(src_col, src_row, dst_col, dst_row);
            break;

        case ROOK:
        case KNIGHT:
        case BISHOP:
        case QUEEN:
        case KING:
        default:
            return can_piece_type_move_to(piece, src_col, src_row, dst_col, dst_row);
            break;
    }
    return false;
}
static bool can_pawn_capture(int src_col, int src_row, int dst_col, int dst_row)
{
    if (dst_row == src_row + 1 && (dst_col == src_row + 1 || dst_col == src_col - 1))
    {
        return true;
    }
    return true;
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
