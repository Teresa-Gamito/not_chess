#include "include/game/board/piece.h"

struct Piece 
{
    PieceType type;
    Color color;

    bool has_moved;
};

static bool can_pawn_move(bool has_moved, int src_col, int src_row, int dst_col, int dst_row);
static bool can_rook_move(int src_col, int src_row, int dst_col, int dst_row);
static bool can_knight_move(int src_col, int src_row, int dst_col, int dst_row);
static bool can_bishop_move(int src_col, int src_row, int dst_col, int dst_row);
static bool can_queen_move(int src_col, int src_row, int dst_col, int dst_row);
static bool can_king_move(int src_col, int src_row, int dst_col, int dst_row);
static bool can_lance_move(int src_col, int src_row, int dst_col, int dst_row);
static bool can_promoted_lance_move(int src_col, int src_row, int dst_col, int dst_row);

static bool can_piece_type_capture(const Piece* piece, int src_col, int src_row, int dst_col, int dst_row);
static bool can_pawn_capture(int src_col, int src_row, int dst_col, int dst_row);




// ========== create ==========
Piece* piece_create(PieceType type, Color color) 
{
    verify_piece_type(type);
    verify(color != WHITE && color != BLACK, "Invalid Color");

    Piece* piece = SDL_malloc(sizeof(Piece));
    verify(piece == NULL, "Piece could not be created: malloc");

    piece->type = type;
    piece->color = color;

    piece->has_moved = false;

    return piece;
}



// ========== destroy ==========
void piece_destroy(Piece* piece) 
{
    verify_piece(piece);

    SDL_free(piece);
}


// ========== movement ==========
bool piece_can_move_to(const Piece* piece, int src_col, int src_row, int dst_col, int dst_row)
{
    verify_piece(piece);

    PieceType type = piece_get_type(piece);
    Color color = piece_get_color(piece);
    bool has_moved = piece_has_moved(piece);
    switch(type)
    {
        case PAWN:
            if (color == BLACK)
                return can_pawn_move(has_moved, src_col, src_row, dst_col, dst_row);
            else
                return can_pawn_move(has_moved, src_col, -src_row, dst_col, -dst_row);

        case ROOK:
            return can_rook_move(src_col, src_row, dst_col, dst_row);

        case KNIGHT:
            return can_knight_move(src_col, src_row, dst_col, dst_row);

        case BISHOP:
            return can_bishop_move(src_col, src_row, dst_col, dst_row);

        case QUEEN:
            return can_queen_move(src_col, src_row, dst_col, dst_row);

        case KING:
            return can_king_move(src_col, src_row, dst_col, dst_row);

        case LANCE:
            if (color == BLACK)
                return can_lance_move(src_col, src_row, dst_col, dst_row);
            else
                return can_lance_move(src_col, -src_row, dst_col, -dst_row);

        case PROMOTED_LANCE:
            if (color == BLACK)
                return can_promoted_lance_move(src_col, src_row, dst_col, dst_row);
            else
                return can_promoted_lance_move(src_col, -src_row, dst_col, -dst_row);

        default:
            break;
    }
    return false;
}
static bool can_pawn_move(bool has_moved, int src_col, int src_row, int dst_col, int dst_row)
{
    if (!has_moved)
    {
        if (src_col == dst_col && dst_row == src_row + 2)
        {
            return true;
        }
    }
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
    if (can_bishop_move(src_col, src_row, dst_col, dst_row) ||
        can_rook_move(src_col, src_row, dst_col, dst_row))
    {
        return true;
    }
    return false;
}
static bool can_king_move(int src_col, int src_row, int dst_col, int dst_row)
{
    if ((SDL_abs(src_col - dst_col) <= 1 && SDL_abs(src_row - dst_row) <= 1))
    {
        return true;
    }
    return false;
}
static bool can_lance_move(int src_col, int src_row, int dst_col, int dst_row)
{
    if (src_col == dst_col && src_row < dst_row)
    {
        return true;
    }
    return false;
}
static bool can_promoted_lance_move(int src_col, int src_row, int dst_col, int dst_row)
{
    if (can_king_move(src_col, src_row, dst_col, dst_row) &&
        !(SDL_abs(src_col - dst_col) == 1 && dst_row == src_row - 1))
    {
        return true;
    }
    return false;
}
bool piece_requires_clear_path(const Piece* piece)
{
    verify_piece(piece);

    PieceType type = piece_get_type(piece);
    switch (type)
    {
        case PAWN:
        case ROOK:
        case BISHOP:
        case QUEEN:
        case LANCE:
            return true;

        case KNIGHT:
        case KING:
        case PROMOTED_LANCE:
        default:
            return false;
    }
}


bool piece_can_capture(const Piece* src_piece, const Piece* dst_piece, int src_col, int src_row, int dst_col, int dst_row)
{
    verify_piece(src_piece);
    verify_piece(dst_piece);

    if (piece_get_color(src_piece) == piece_get_color(dst_piece))
    {
        return false;
    }
    if (!can_piece_type_capture(src_piece, src_col, src_row, dst_col, dst_row))
    {
        return false;
    }
    return true;
}
static bool can_piece_type_capture(const Piece* piece, int src_col, int src_row, int dst_col, int dst_row)
{
    PieceType type = piece_get_type(piece);
    Color color = piece_get_color(piece);
    switch (type) 
    {
        case PAWN:
            if (color == BLACK)
                return can_pawn_capture(src_col, src_row, dst_col, dst_row);
            else
                return can_pawn_capture(src_col, -src_row, dst_col, -dst_row);

        case ROOK:
        case KNIGHT:
        case BISHOP:
        case QUEEN:
        case KING:
        case LANCE:
        case PROMOTED_LANCE:
        default:
            return piece_can_move_to(piece, src_col, src_row, dst_col, dst_row);
    }
    return false;
}
static bool can_pawn_capture(int src_col, int src_row, int dst_col, int dst_row)
{
    if (dst_row == src_row + 1 && (dst_col == src_col + 1 || dst_col == src_col - 1))
    {
        return true;
    }
    return false;
}

void piece_promote(Piece* piece)
{
    verify_piece(piece);

    PieceType type = piece_get_type(piece);
    switch (type)
    {
        case PAWN:
            piece->type = QUEEN;
            break;

        case LANCE:
            piece->type = PROMOTED_LANCE;
            break;

        default:
            break;
    }
}



// ========== set ==========
void piece_set_type(Piece* piece, PieceType type) 
{
    verify_piece(piece);
    verify_piece_type(type);

    piece->type = type;
}
void piece_set_colour(Piece* piece, Color color)
{
    verify_piece(piece);
    verify(color != WHITE || color != BLACK, "Invalid PieceColor");

    piece->color = color;
}
void piece_set_moved(Piece* piece)
{
    verify_piece(piece);

    piece->has_moved = true;
}



// ========== get ==========
PieceType piece_get_type(const Piece* piece)
{
    verify_piece(piece);

    return piece->type;
}
Color piece_get_color(const Piece* piece)
{
    verify_piece(piece);

    return piece->color;
}
int piece_get_points(const Piece* piece)
{
    verify_piece(piece);

    PieceType type = piece_get_type(piece);
    switch (type) 
    {
        case PAWN:
            return POINTS_PAWN;

        case ROOK:
            return POINTS_ROOK;

        case KNIGHT:
            return POINTS_KNIGHT;

        case BISHOP:
            return POINTS_BISHOP;

        case QUEEN:
            return POINTS_QUEEN;

        case KING:
            return POINTS_KING;

        case LANCE:
            return POINTS_LANCE;

        case PROMOTED_LANCE:
            return POINTS_PROMOTED_LANCE;

        default:
            return 0;
    }
    return 0;
}
const char* piece_type_get_name(PieceType type)
{
    switch (type)
    {
        case PAWN:
            return "Pawn";
        case ROOK:
            return "Rook";
        case KNIGHT:
            return "Knight";
        case BISHOP:
            return "Bishop";
        case QUEEN:
            return "Queen";
        case KING:
            return "King";
        case LANCE:
            return "Lance";
        case PROMOTED_LANCE:
            return "Promoted Lance";
        default:
            return "Invalid type";
    }
}
bool piece_has_moved(const Piece* piece)
{
    verify_piece(piece);

    return piece->has_moved;
}



static void destroy(void* piece)
{
    piece_destroy(piece);
}
static TypeOps ops =
    {
        destroy
    };
TypeOps* piece_ops()
{
    return &ops;
}



void verify_piece(const Piece* piece)
{
    verify(piece == NULL, "Piece does not exist");
}
void verify_piece_type(PieceType type)
{
    verify(type < 0 || type >= PIECE_TYPE_COUNT, "Invalid PieceType");
}
