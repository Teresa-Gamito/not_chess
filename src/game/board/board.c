#include "include/game/board/board.h"

static const char board_default_layout[] = "RNBQKBNRPPPPPPPP00000000000000000000000000000000pppppppprnbqkbnr";

static int get_vector_index(const Board* board, Pos pos);
static bool board_piece_has_clear_path(const Board* board, Pos src, Pos dst);
static int board_piece_capture(Piece* piece);
static bool board_can_piece_promote(Board* board, Pos pos);

static void board_add_row(Board* board);
static void board_add_col(Board* board);

struct Board
{
    int col_num;
    int row_num;

    Vector* tiles;
    Vector* pieces;

    RuleList* rules;
};

Board* board_create(RuleList* rules, int col_num, int row_num)
{
    verify(col_num <= 0 || row_num <= 0, "Invalid board starting size");

    Board* board = SDL_malloc(sizeof(Board));
    verify(board == NULL, "Board could not be created: malloc");

    board->col_num = col_num;
    board->row_num = row_num;

    board->tiles = vector_create();
    board->pieces = vector_create();

    for (int col = 0; col < col_num; col++)
    {
        for (int row = 0; row < row_num; row++)
        {
            Pos pos = {col, row};

            Color color = tile_get_default_color(pos);
            Tile* tile = tile_create(TILE_NONE, color);
            vector_add(board->tiles, tile);

            vector_add(board->pieces, NULL);
        }
    }

    board->rules = rules;

    return board;
}

void board_destroy(Board* board)
{
    verify_board(board);

    for (int i = 0; i < vector_get_size(board->tiles); i++)
    {
        Tile* tile = vector_get_at(board->tiles, i);
        tile_destroy(tile);
    }
    vector_destroy(board->tiles);

    for (int i = 0; i < vector_get_size(board->pieces); i++)
    {
        Piece* piece = vector_get_at(board->pieces, i);
        if (piece == NULL) continue;
        piece_destroy(piece);
    }
    vector_destroy(board->pieces);

    SDL_free(board);
}

void board_set_default(Board* board)
{
    verify_board(board);

    PieceType type;
    Color color;
    for (int col = 0; col < board_get_col_num(board); col++)
    {
        for (int row = 0; row < board_get_row_num(board); row++)
        {
            Pos pos = {col, row};
            int index = get_vector_index(board, pos);
            switch (board_default_layout[index])
            {
                case 'P':
                case 'p':
                    type = PAWN;
                    break;
                case 'R':
                case 'r':
                    type = ROOK;
                    break;
                case 'N':
                case 'n':
                    type = KNIGHT;
                    break;
                case 'B':
                case 'b':
                    type = BISHOP;
                    break;
                case 'Q':
                case 'q':
                    type = QUEEN;
                    break;
                case 'K':
                case 'k':
                    type = KING;
                    break;
                case 'L':
                case 'l':
                    type = LANCE;
                    break;
                default:
                    continue;
            }
            if (board_default_layout[index] > 'a')
            {
                color = WHITE;
            }
            else color = BLACK;

            Piece* piece = piece_create(type, color);
            board_add_piece_at(board, piece, pos);
        }
    }
}

int board_get_col_num(const Board* board)
{
    verify_board(board);

    return board->col_num;
}

int board_get_row_num(const Board* board)
{
    verify_board(board);

    return board->row_num;
}

Piece* board_get_piece_at(const Board* board, Pos pos)
{
    verify_board(board);
    verify_board_pos(board, pos);

    int index = get_vector_index(board, pos);
    return vector_get_at(board->pieces, index);
}

Tile* board_get_tile_at(const Board* board, Pos pos)
{
    verify_board(board);
    verify_board_pos(board, pos);

    int index = get_vector_index(board, pos);
    return vector_get_at(board->tiles, index);
}

void board_add_piece_at(Board* board, Piece* piece, Pos pos)
{
    verify_board(board);
    verify_piece(piece);
    verify_board_pos(board, pos);
    verify(board_has_piece_at(board, pos), "Could not add piece, position already occupied");

    int index = get_vector_index(board, pos);
    vector_set_at(board->pieces, index, piece);
}

void board_piece_remove(Board* board, Pos pos)
{
    verify_board(board);
    verify_board_pos(board, pos);
    verify(!board_has_piece_at(board, pos), "Piece does not exist");

    piece_destroy(board_get_piece_at(board, pos));
    int index = get_vector_index(board, pos);
    vector_set_at(board->pieces, index, NULL);
}

bool board_has_piece_at(const Board* board, Pos pos)
{
    verify_board(board);
    verify_board_pos(board, pos);

    return board_get_piece_at(board, pos) != NULL;
}

Pos board_piece_get_pos(const Board* board, const Piece* piece)
{
    verify_board(board);
    verify_piece(piece);

    int pos = vector_search(board->pieces, piece);
    return (Pos){pos % board->col_num, pos / board->col_num};
}

bool board_can_piece_move_to(const Board* board, Pos src, Pos dst)
{
    verify_board(board);
    verify_board_pos(board, src);
    verify_board_pos(board, dst);
    verify(!board_has_piece_at(board, src), "Piece does not exist");

    Piece* piece = board_get_piece_at(board, src);
    if (board_has_piece_at(board, dst) && !board_can_piece_capture(board, src, dst))
    {
        return false;
    }
    if (!piece_can_move_to(piece, src, dst))
    {
        return false;
    }
    if (tile_get_type(board_get_tile_at(board, dst)) == TILE_WALL)
    {
        return false;
    }
    if (piece_requires_clear_path(piece) && !board_piece_has_clear_path(board, src, dst))
    {
        return false;
    }
    return true;
}

bool board_can_piece_capture(const Board* board, Pos src, Pos dst)
{
    verify(!board_has_piece_at(board, src), "Piece does not exist");
    verify(!board_has_piece_at(board, dst), "Piece does not exist");

    Piece* src_piece = board_get_piece_at(board, src);
    Piece* dst_piece = board_get_piece_at(board, dst);
    if (!piece_can_capture(src_piece, dst_piece, src, dst))
    {
        return false;
    }
    if (piece_requires_clear_path(src_piece) && !board_piece_has_clear_path(board, src, dst))
    {
        return false;
    }
    return true;
}

static bool board_piece_has_clear_path(const Board* board, Pos src, Pos dst)
{
    // get the distance between the source tile and the next one
    Pos step = {sign(dst.col - src.col), sign(dst.row - src.row)};
    // get the next position
    Pos next = pos_add(src, step);

    // if the next position is the destination then there was no occupied tile along the way
    if (pos_equals(next, dst))
    {
        return true;
    }
    // if the position is occupied then there is no clear path
    // this excludes the starting and destination positions
    if (board_has_piece_at(board, next) || tile_get_type(board_get_tile_at(board, next)) == TILE_WALL)
    {
        return false;
    }
    // verify the next position
    return board_piece_has_clear_path(board, next, dst);
}

static bool board_can_piece_promote(Board* board, Pos pos)
{
    Piece* piece = board_get_piece_at(board, pos);
    Color color = piece_get_color(piece);

    if (color == WHITE && pos.row == 0)
    {
        return true;
    }
    if (color == BLACK && pos.row == board_get_row_num(board) - 1)
    {
        return true;
    }
    return false;
}

int board_piece_move_to(Board* board, Pos src, Pos dst) 
{
    verify_board(board);
    verify_board_pos(board, src);
    verify_board_pos(board, dst);
    verify(!board_has_piece_at(board, src), "Piece does not exist");

    Piece* piece = board_get_piece_at(board, src);
    int points = 0;

    if (board_has_piece_at(board, dst))
    {
        Piece* dst_piece = board_get_piece_at(board, dst);
        points = board_piece_capture(dst_piece);
        if (board_has_rule(board, RULE_PAWN_PROMOTION_CHANCE))
        {
            if (piece_get_type(piece) == PAWN)
            {
                if (SDL_randf() < CHANCE_RULE_PAWN_PROMOTE)
                {
                    piece_set_type(piece, QUEEN);
                }
            }
        }
    }

    int src_pos = get_vector_index(board, src);
    vector_set_at(board->pieces, src_pos, NULL);
    int dst_pos = get_vector_index(board, dst);
    vector_set_at(board->pieces, dst_pos, piece);

    if (board_can_piece_promote(board, dst))
    {
        piece_promote(piece);
    }

    piece_set_moved(piece);

    return points;
}

static int board_piece_capture(Piece* piece)
{
    int points = piece_get_points(piece);
    piece_destroy(piece);
    return points;
}

void board_expand(Board* board)
{
    verify_board(board);

    board_add_col(board);
    board_add_col(board);
    board_add_row(board);
    board_add_row(board);

    int col_num = board->col_num;
    int row_num = board->row_num;

    // adjust piece position
    for (int col = col_num - 1; col >= 0; col--)
    {
        for (int row = row_num - 1; row >= 0; row--)
        {
            Pos new = {col, row};
            Pos old = {col - 1, row - 1};
            if (old.col < 0) old.col = col_num - 1;
            if (old.row < 0) old.row = row_num - 1;

            int old_pos = get_vector_index(board, old);
            int new_pos = get_vector_index(board, new);

            vector_swap(board->pieces, old_pos, new_pos);
            vector_swap(board->tiles, old_pos, new_pos);
        }
    }
}

static void board_add_col(Board* board)
{
    board->col_num++;
    int col_num = board->col_num;
    int row_num = board->row_num;

    // increase stack size
    for (int row = 0; row < row_num; row++)
    {
        vector_add(board->tiles, NULL);
        vector_add(board->pieces, NULL);
    }

    // adjust positions
    int old_pos;
    int new_pos;
    for (int i = col_num * row_num - 1; i >= col_num; i--)
    {
        old_pos = i - (i / col_num);
        new_pos = i;
        vector_swap(board->pieces, old_pos, new_pos);
        vector_swap(board->tiles, old_pos, new_pos);
    }

    // add new spaces
    for (int row = 0; row < row_num; row++)
    {
        Color color = tile_get_default_color((Pos){col_num - 1, row});
        Tile* tile = tile_create(TILE_NONE, color);
        int pos = get_vector_index(board, (Pos){col_num - 1, row});
        vector_set_at(board->tiles, pos, tile);
    }
}

static void board_add_row(Board* board)
{
    verify_board(board);

    board->row_num++;
    int col_num = board->col_num;
    int row_num = board->row_num;

    for (int col = 0; col < col_num; col++)
    {
        Color color = tile_get_default_color((Pos){col, row_num - 1});
        Tile* tile = tile_create(TILE_NONE, color);
        vector_add(board->tiles, tile);
        vector_add(board->pieces, NULL);
    }
}

Pos board_tile_get_pos(const Board* board, const Tile* tile)
{
    verify_board(board);
    verify_tile(tile);

    int pos = vector_search(board->tiles, tile);
    return (Pos){pos % board->col_num, pos / board->col_num};
}

void board_add_rule(Board* board, Rule rule)
{
    rulelist_add(board->rules, rule);
}

bool board_has_rule(const Board* board, Rule rule)
{
    return rulelist_has(board->rules, rule);
}

bool is_check_mate(const Board* board, Color color)
{
    for (int col = 0; col < board_get_col_num(board); col++)
    {
        for (int row = 0; row < board_get_row_num(board); row++)
        {
            Pos pos = (Pos){col, row};
            if (!board_has_piece_at(board, pos))
            {
                continue;
            }
            Piece* piece = board_get_piece_at(board, pos);
            if (piece_get_type(piece) == KING && piece_get_color(piece) == color_get_opposite(color))
            {
                return false;
            }
        }
    }
    return true;
}

static int get_vector_index(const Board* board, Pos pos)
{
    return pos.col + pos.row * board->col_num;
}

void verify_board_pos(const Board* board, Pos pos)
{
    verify(pos.col < 0 || pos.col >= board->col_num, "Invalid column");
    verify(pos.row < 0 || pos.row >= board->row_num, "Invalid row");
}

void verify_board(const Board* board)
{
    verify(board == NULL, "Board does not exist");
}
