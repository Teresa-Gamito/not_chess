#include "include/game/board/board_elements/piece.h"

struct Piece 
{
    PieceType type;
    Color color;

    bool has_moved;
};

static bool can_pawn_move(bool has_moved, Pos src, Pos dst);
static bool can_rook_move(Pos src, Pos dst);
static bool can_knight_move(Pos src, Pos dst);
static bool can_bishop_move(Pos src, Pos dst);
static bool can_queen_move(Pos src, Pos dst);
static bool can_king_move(Pos src, Pos dst);
static bool can_lance_move(Pos src, Pos dst);
static bool can_promoted_lance_move(Pos src, Pos dst);

static bool can_piece_type_capture(const Piece* piece, Pos src, Pos dst);
static bool can_pawn_capture(Pos src, Pos dst);

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

void piece_destroy(Piece* piece) 
{
    verify_piece(piece);
    SDL_free(piece);
}

void piece_set_type(Piece* piece, PieceType type) 
{
    verify_piece(piece);
    verify_piece_type(type);

    piece->type = type;
}

void piece_set_color(Piece* piece, Color color)
{
    verify_piece(piece);
    verify_color(color);
    piece->color = color;
}

void piece_set_moved(Piece* piece)
{
    verify_piece(piece);
    piece->has_moved = true;
}

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

bool piece_has_moved(const Piece* piece)
{
    verify_piece(piece);

    return piece->has_moved;
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

        case P_LANCE:
            return POINTS_P_LANCE;

        default:
            return 0;
    }
    return 0;
}

const char* piece_type_get_name(PieceType type)
{
    verify_piece_type(type);

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
        case P_LANCE:
            return "Promoted Lance";
        default:
            return "Invalid type";
    }
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
        case P_LANCE:
        default:
            return false;
    }
}

bool piece_can_move_to(const Piece* piece, Pos src, Pos dst)
{
    verify_piece(piece);

    PieceType type = piece_get_type(piece);
    Color color = piece_get_color(piece);
    bool has_moved = piece_has_moved(piece);
    switch(type)
    {
        case PAWN:
            if (color == WHITE)
                return can_pawn_move(has_moved, pos_mirror_y(src), pos_mirror_y(dst));
            else
                return can_pawn_move(has_moved, src, dst);

        case ROOK:
            return can_rook_move(src, dst);

        case KNIGHT:
            return can_knight_move(src, dst);

        case BISHOP:
            return can_bishop_move(src, dst);

        case QUEEN:
            return can_queen_move(src, dst);

        case KING:
            return can_king_move(src, dst);

        case LANCE:
            if (color == WHITE)
                return can_lance_move(pos_mirror_y(src), pos_mirror_y(dst));
            else
                return can_lance_move(src, dst);

        case P_LANCE:
            if (color == WHITE)
                return can_promoted_lance_move(pos_mirror_y(src), pos_mirror_y(dst));
            else
                return can_promoted_lance_move(src, dst);

        default:
            break;
    }
    return false;
}

static bool can_pawn_move(bool has_moved, Pos src, Pos dst)
{
    if (pos_equals(dst, (Pos){src.col, src.row + 1}))
    {
        return true;
    }
    if (pos_equals(dst, (Pos){src.col, src.row + 2}) && !has_moved)
    {
        return true;
    }
    return false;
}

static bool can_rook_move(Pos src, Pos dst)
{
    if (src.col == dst.col || src.row == dst.row)
    {
        return true;
    }
    return false;
}

static bool can_knight_move(Pos src, Pos dst)
{
    if ((SDL_abs(src.col - dst.col) == 1 && SDL_abs(src.row - dst.row) == 2) ||
        (SDL_abs(src.col - dst.col) == 2 && SDL_abs(src.row - dst.row) == 1))
    {
        return true;
    }
    return false;
}

static bool can_bishop_move(Pos src, Pos dst)
{
    if (dst.col - src.col == dst.row - src.row || dst.row - src.row == -dst.col + src.col)
    {
        return true;
    }
    return false;
}

static bool can_queen_move(Pos src, Pos dst)
{
    if (can_bishop_move(src, dst) ||
        can_rook_move(src, dst))
    {
        return true;
    }
    return false;
}

static bool can_king_move(Pos src, Pos dst)
{
    if ((SDL_abs(src.col - dst.col) <= 1 && SDL_abs(src.row - dst.row) <= 1))
    {
        return true;
    }
    return false;
}

static bool can_lance_move(Pos src, Pos dst)
{
    if (src.col == dst.col && src.row < dst.row)
    {
        return true;
    }
    return false;
}

static bool can_promoted_lance_move(Pos src, Pos dst)
{
    if (can_king_move(src, dst) &&
        !(SDL_abs(src.col - dst.col) == 1 && dst.row == src.row - 1))
    {
        return true;
    }
    return false;
}

bool piece_can_capture(const Piece* src_piece, const Piece* dst_piece, Pos src, Pos dst)
{
    verify_piece(src_piece);
    verify_piece(dst_piece);

    if (piece_get_color(src_piece) == piece_get_color(dst_piece))
    {
        return false;
    }
    if (!can_piece_type_capture(src_piece, src, dst))
    {
        return false;
    }
    return true;
}

static bool can_piece_type_capture(const Piece* piece, Pos src, Pos dst)
{
    PieceType type = piece_get_type(piece);
    Color color = piece_get_color(piece);
    switch (type) 
    {
        case PAWN:
            if (color == BLACK)
                return can_pawn_capture(src, dst);
            else
                return can_pawn_capture(pos_mirror_y(src), pos_mirror_y(dst));

        case ROOK:
        case KNIGHT:
        case BISHOP:
        case QUEEN:
        case KING:
        case LANCE:
        case P_LANCE:
        default:
            return piece_can_move_to(piece, src, dst);
    }
    return false;
}

static bool can_pawn_capture(Pos src, Pos dst)
{
    if (dst.row == src.row + 1 && (dst.col == src.col + 1 || dst.col == src.col - 1))
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
            piece->type = P_LANCE;
            break;
        default:
            break;
    }
}

void verify_piece(const Piece* piece)
{
    verify(piece == NULL, "Piece does not exist");
}

void verify_piece_type(PieceType type)
{
    verify(type < 0 || type >= PIECE_TYPE_COUNT, "Invalid PieceType");
}
