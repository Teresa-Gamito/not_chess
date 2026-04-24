#include "include/game/board/board.h"
#include "game/board/piece.h"
#include "helper_functions/error_handling.h"

static bool board_piece_has_clear_path(const Board* board, int src_col, int src_row, int dst_col, int dst_row);

struct Board
{
    int col_num;
    int row_num;

    Tile** tiles;
    Piece** pieces;

    Player* player1;
    Player* player2;
    Player** active_player;
};

Board* board_create(int col_num, int row_num)
{
    Board* board = SDL_malloc(sizeof(Board));
    verify(board == NULL, "Board could not be created: malloc");

    board->col_num = col_num;
    board->row_num = row_num;

    board->tiles = SDL_calloc(col_num * row_num, sizeof(Tile*));
    verify(board->tiles == NULL, "Tiles could not be created: malloc");

    board->pieces = SDL_calloc(col_num * row_num, sizeof(Piece*));
    verify(board->pieces == NULL, "Pieces could not be created: malloc");

    Tile* tile;
    for (int col = 0; col < col_num; col++)
    {
        for (int row = 0; row < row_num; row++)
        {
            tile = tile_create(TILE_NONE);
            board->tiles[col + row * board->col_num] = tile;
            board->pieces[col + row * board->col_num] = NULL;
        }
    }

    board->player1 = NULL;
    board->player2 = NULL;
    board->active_player = &board->player1;

    return board;
}

void board_destroy(Board* board)
{
    verify(board == NULL, "Board does not exist");

    int col_num = board->col_num;
    int row_num = board->row_num;

    for (int col = 0; col < col_num; col++)
    {
        for (int row = 0; row < row_num; row++)
        {
            Tile* tile = board_get_tile_at(board, col, row);
            tile_destroy(tile);

            if (!board_has_piece_at(board, col, row))
            {
                continue;
            }
            Piece* piece = board_get_piece_at(board, col, row);
            piece_destroy(piece);
        }
    }
    SDL_free(board->tiles);
    SDL_free(board->pieces);

    if (board->player1 != NULL) 
    {
        player_destroy(board->player1);
    }
    if (board->player2 != NULL) 
    {
        player_destroy(board->player2);
    }

    SDL_free(board);
}

void board_set_default(Board* board)
{
    verify(board == NULL, "Board does not exist");

    board->player1 = player_create(WHITE, 0);
    board->player2 = player_create(BLACK, 0);

    board_add_piece_at(board, piece_create(ROOK, BLACK), 0, 0);
    board_add_piece_at(board, piece_create(KNIGHT, BLACK), 1, 0);
    board_add_piece_at(board, piece_create(BISHOP, BLACK), 2, 0);
    board_add_piece_at(board, piece_create(QUEEN, BLACK), 3, 0);
    board_add_piece_at(board, piece_create(KING, BLACK), 4, 0);
    board_add_piece_at(board, piece_create(BISHOP, BLACK), 5, 0);
    board_add_piece_at(board, piece_create(KNIGHT, BLACK), 6, 0);
    board_add_piece_at(board, piece_create(ROOK, BLACK), 7, 0);
    board_add_piece_at(board, piece_create(PAWN, BLACK), 0, 1);
    board_add_piece_at(board, piece_create(PAWN, BLACK), 1, 1);
    board_add_piece_at(board, piece_create(PAWN, BLACK), 2, 1);
    board_add_piece_at(board, piece_create(PAWN, BLACK), 3, 1);
    board_add_piece_at(board, piece_create(PAWN, BLACK), 4, 1);
    board_add_piece_at(board, piece_create(PAWN, BLACK), 5, 1);
    board_add_piece_at(board, piece_create(PAWN, BLACK), 6, 1);
    board_add_piece_at(board, piece_create(PAWN, BLACK), 7, 1);

    board_add_piece_at(board, piece_create(ROOK, WHITE), 0, 7);
    board_add_piece_at(board, piece_create(KNIGHT, WHITE), 1, 7);
    board_add_piece_at(board, piece_create(BISHOP, WHITE), 2, 7);
    board_add_piece_at(board, piece_create(QUEEN, WHITE), 3, 7);
    board_add_piece_at(board, piece_create(KING, WHITE), 4, 7);
    board_add_piece_at(board, piece_create(BISHOP, WHITE), 5, 7);
    board_add_piece_at(board, piece_create(KNIGHT, WHITE), 6, 7);
    board_add_piece_at(board, piece_create(ROOK, WHITE), 7, 7);
    board_add_piece_at(board, piece_create(PAWN, WHITE), 0, 6);
    board_add_piece_at(board, piece_create(PAWN, WHITE), 1, 6);
    board_add_piece_at(board, piece_create(PAWN, WHITE), 2, 6);
    board_add_piece_at(board, piece_create(PAWN, WHITE), 3, 6);
    board_add_piece_at(board, piece_create(PAWN, WHITE), 4, 6);
    board_add_piece_at(board, piece_create(PAWN, WHITE), 5, 6);
    board_add_piece_at(board, piece_create(PAWN, WHITE), 6, 6);
    board_add_piece_at(board, piece_create(PAWN, WHITE), 7, 6);
}


bool board_can_add_piece_at(Board* board, int col, int row)
{
    verify(board == NULL, "Board does not exist");
    verify(col < 0 || col > board->col_num, "Invalid board position");
    verify(row < 0 || row > board->row_num, "Invalid board position");

    if (board_has_piece_at(board, col, row))
    {
        return false;
    }
    return true;
}
void board_add_piece_at(Board* board, Piece* piece, int col, int row)
{
    verify(board == NULL, "Board does not exist");
    verify(piece == NULL, "Piece does not exist");
    verify(col < 0 || col > board->col_num, "Invalid board position");
    verify(row < 0 || row > board->row_num, "Invalid board position");
    verify(board_has_piece_at(board, col, row), "Could not add piece, position already occupied");

    board->pieces[col + row * board->col_num] = piece;
}
void board_piece_remove(Board* board, int col, int row)
{
    verify(board == NULL, "Board does not exist");
    verify(col < 0 || col > board->col_num, "Invalid board position");
    verify(row < 0 || row > board->row_num, "Invalid board position");
    verify(!board_has_piece_at(board, col, row), "Piece does not exist");

    Piece* piece = board_get_piece_at(board, col, row);
    piece_destroy(piece);
    board->pieces[col + row * board->col_num] = NULL;
}
bool board_has_piece_at(const Board* board, int col, int row)
{
    verify(board == NULL, "Board does not exist");
    verify(col < 0 || col >= board->col_num , "Invalid position");
    verify(row < 0 || row >= board->row_num , "Invalid position");

    return board_get_piece_at(board, col, row) != NULL;
}
Piece* board_get_piece_at(const Board* board, int col, int row)
{
    verify(board == NULL, "Board does not exist");
    verify(col < 0 || col > board->col_num, "Invalid board position");
    verify(row < 0 || row > board->row_num, "Invalid board position");

    return board->pieces[col + row * board->col_num];
}
int board_piece_get_col(const Board* board, const Piece* piece)
{
    verify(board == NULL, "Board does not exist");
    verify(piece == NULL, "Piece does not exist");

    for (int col = 0; col < board->col_num; col++)
    {
        for (int row = 0; row < board->row_num; row++)
        {
            if (piece == board_get_piece_at(board, col, row))
            {
                return col;
            }
        }
    }
    return 0;
}
int board_piece_get_row(const Board* board, const Piece* piece)
{
    verify(board == NULL, "Board does not exist");
    verify(piece == NULL, "Piece does not exist");

    for (int col = 0; col < board->col_num; col++)
    {
        for (int row = 0; row < board->row_num; row++)
        {
            if (piece == board_get_piece_at(board, col, row))
            {
                return row;
            }
        }
    }
    return 0;
}
bool board_can_piece_move_to(Board* board, int src_col, int src_row, int dst_col, int dst_row)
{
    verify(board == NULL, "Board does not exist");
    verify(!board_has_piece_at(board, src_col, src_row), "Piece does not exist");

    Piece* piece = board_get_piece_at(board, src_col, src_row);
    if (board_has_piece_at(board, dst_col, dst_row))
    {
        return false;
    }
    if (!piece_can_move_to(piece, src_col, src_row, dst_col, dst_row))
    {
        return false;
    }
    if (piece_requires_clear_path(piece) && !board_piece_has_clear_path(board, src_col, src_row, dst_col, dst_row))
    {
        return false;
    }

    return true;
}
void board_piece_move_to(Board* board, int src_col, int src_row, int dst_col, int dst_row)
{
    verify(board == NULL, "Board does not exist");
    verify(src_col < 0 || src_col >= board->col_num , "Invalid position");
    verify(src_row < 0 || src_row >= board->row_num , "Invalid position");
    verify(dst_col < 0 || dst_col >= board->col_num , "Invalid position");
    verify(dst_row < 0 || dst_row >= board->row_num , "Invalid position");
    verify(!board_has_piece_at(board, src_col, src_row), "Piece does not exist");

    Piece* piece = board_get_piece_at(board, src_col, src_row);

    if (board_has_piece_at(board, dst_col, dst_row))
    {
        Piece* dst_piece = board_get_piece_at(board, dst_col, dst_row);
        if (!piece_can_capture(piece, dst_piece, src_col, src_row, dst_col, dst_row))
        {
            return;
        }
        if (!board_can_piece_capture(board, src_col, src_row, dst_col, dst_row))
        {
            return;
        }
        piece_destroy(board_get_piece_at(board, dst_col, dst_row));
    }

    board->pieces[dst_col + dst_row * board->col_num] = board->pieces[src_col + src_row * board->col_num];
    board->pieces[src_col + src_row * board->col_num] = NULL;
    piece_set_moved(piece);
}
bool board_can_piece_capture(Board* board, int src_col, int src_row, int dst_col, int dst_row)
{
    verify(!board_has_piece_at(board, src_col, src_row), "Piece does not exist");
    verify(!board_has_piece_at(board, dst_col, dst_row), "Piece does not exist");

    Piece* src_piece = board_get_piece_at(board, src_col, src_row);
    Piece* dst_piece = board_get_piece_at(board, dst_col, dst_row);
    if (!piece_can_capture(src_piece, dst_piece, src_col, src_row, dst_col, dst_row))
    {
        return false;
    }
    if (piece_requires_clear_path(src_piece) && !board_piece_has_clear_path(board, src_col, src_row, dst_col, dst_row))
    {
        return false;
    }
    return true;

}
void board_piece_capture(Board* board, Piece* piece)
{
    verify(piece == NULL, "Piece does not exist");

    Player* player = board_get_active_player(board);
    player_add_points(player, piece_get_points(piece));
}
static bool board_piece_has_clear_path(const Board* board, int src_col, int src_row, int dst_col, int dst_row)
{
    verify(board == NULL, "Board does not exist");
    verify(src_col < 0 || src_col >= board->col_num , "Invalid position");
    verify(src_row < 0 || src_row >= board->row_num , "Invalid position");
    verify(dst_col < 0 || dst_col >= board->col_num , "Invalid position");
    verify(dst_row < 0 || dst_row >= board->row_num , "Invalid position");
    verify(!board_has_piece_at(board, src_col, src_row), "Piece does not exist");

    Piece* piece = board_get_piece_at(board, src_col, src_row);

    if (!piece_requires_clear_path(piece))
    {
        return true;
    }

    int col_size = SDL_abs(dst_col - src_col);
    int row_size = SDL_abs(dst_row - src_row);
    int col_dir = sign(dst_col - src_col);
    int row_dir = sign(dst_row - src_row);

    int col = 0;
    int row = 0;
    int board_col, board_row;
    while (col <= col_size && row <= row_size)
    {
        board_col = src_col + (col * col_dir);
        board_row = src_row + (row * row_dir);

        if ((board_col == dst_col && board_row == dst_row) || 
            (board_col == src_col && board_row == src_row))
        { /* do nothing */ }
        else if (board_has_piece_at(board, board_col, board_row))
        {
            return false;
        }

        col += SDL_abs(col_dir);
        row += SDL_abs(row_dir);
    }
    return true;
}



Tile* board_get_tile_at(const Board* board, int col, int row)
{
    verify(board == NULL, "Board does not exist");
    verify(col < 0 || col > board->col_num, "Invalid board position");
    verify(row < 0 || row > board->row_num, "Invalid board position");

    return board->tiles[col + row * board->col_num];
}
int board_tile_get_col(const Board* board, const Tile* tile)
{
    verify(board == NULL, "Board does not exist");
    verify(tile == NULL, "Tile does not exist");

    for (int col = 0; col < board->col_num; col++)
    {
        for (int row = 0; row < board->row_num; row++)
        {
            if (tile == board_get_tile_at(board, col, row))
            {
                return col;
            }
        }
    }
    return 0;
}
int board_tile_get_row(const Board* board, const Tile* tile)
{
    verify(board == NULL, "Board does not exist");
    verify(tile == NULL, "Tile does not exist");

    for (int col = 0; col < board->col_num; col++)
    {
        for (int row = 0; row < board->row_num; row++)
        {
            if (tile == board_get_tile_at(board, col, row))
            {
                return row;
            }
        }
    }
    return 0;
}



int board_get_col_num(const Board* board)
{
    verify(board == NULL, "Board does not exist");

    return board->col_num;
}
int board_get_row_num(const Board* board)
{
    verify(board == NULL, "Board does not exist");

    return board->row_num;
}

void advance_turn(Board* board)
{
    verify(board == NULL, "Board does not exist");

    if (*board->active_player == board->player1)
    {
        board->active_player = &board->player2;
    }
    else
    {
        board->active_player = &board->player1;
    }
}
bool is_player_side_of_board(const Board* board, const Player* player, int row)
{
    verify(board == NULL, "Board does not exist");
    verify(player == NULL, "Player does not exist");

    if (player_get_color(player) == WHITE && row >= board_get_row_num(board) / 2)
    {
        return true;
    }
    if (player_get_color(player) == BLACK && row + 1 <= board_get_row_num(board) / 2)
    {
        return true;
    }
    return false;
}
Player* board_get_active_player(const Board* board)
{
    verify(board == NULL, "Board does not exist");

    return *board->active_player;
}
Player* board_get_opponent(const Board* board)
{
    verify(board == NULL, "Board does not exist");

    return *board->active_player == board->player1 ? board->player2 : board->player1;
}

Player* board_get_player_white(const Board* board)
{
    return board->player1;
}
Player* board_get_player_black(const Board* board)
{
    return board->player2;
}
