#include "../../../include/game/board/board.h"
#include <SDL3/SDL_stdinc.h>

struct BoardState
{
    // size
    int width;
    int height;

    // tiles
    Tile** tiles;

    // pieces
    Piece** pieces;
};



// ========== create ==========
BoardState* board_create()
{
    BoardState* board = (BoardState*)SDL_malloc(sizeof(BoardState));

    board->width = BOARD_STARTING_WIDTH;
    board->height = BOARD_STARTING_HEIGHT;

    board->tiles = SDL_malloc(sizeof(Tile*) * board->width * board->height);
    board->pieces = SDL_malloc(sizeof(Piece*) * board->width * board->height);

    for (int i = 0; i < board->width; i++)
    {
        for (int j = 0; j < board->height; j++)
        {
            board->tiles[i + j * board->width] = tile_create(TILE_NONE);
            board->pieces[i + j * board->width] = NULL;
        }
    }

    board_set_starting_layout(board);

    return board;
}
void board_set_starting_layout(BoardState* board)
{
    // TODO: replace with map
    board_add_piece_at(board, piece_create(KING, PIECE_WHITE), 4, 0);
    board_add_piece_at(board, piece_create(QUEEN, PIECE_WHITE), 3, 0);
    board_add_piece_at(board, piece_create(BISHOP, PIECE_WHITE), 2, 0);
    board_add_piece_at(board, piece_create(BISHOP, PIECE_WHITE), 5, 0);
    board_add_piece_at(board, piece_create(KNIGHT, PIECE_WHITE), 1, 0);
    board_add_piece_at(board, piece_create(KNIGHT, PIECE_WHITE), 6, 0);
    board_add_piece_at(board, piece_create(ROOK, PIECE_WHITE), 0, 0);
    board_add_piece_at(board, piece_create(ROOK, PIECE_WHITE), 7, 0);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 0, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 1, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 2, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 3, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 4, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 5, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 6, 1);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 7, 1);

    board_add_piece_at(board, piece_create(KING, PIECE_WHITE), 4, 7);
    board_add_piece_at(board, piece_create(QUEEN, PIECE_WHITE), 3, 7);
    board_add_piece_at(board, piece_create(BISHOP, PIECE_WHITE), 2, 7);
    board_add_piece_at(board, piece_create(BISHOP, PIECE_WHITE), 5, 7);
    board_add_piece_at(board, piece_create(KNIGHT, PIECE_WHITE), 1, 7);
    board_add_piece_at(board, piece_create(KNIGHT, PIECE_WHITE), 6, 7);
    board_add_piece_at(board, piece_create(ROOK, PIECE_WHITE), 0, 7);
    board_add_piece_at(board, piece_create(ROOK, PIECE_WHITE), 7, 7);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 0, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 1, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 2, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 3, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 4, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 5, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 6, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_WHITE), 7, 6);
}



// ========== destroy ==========
void board_destroy(BoardState* board)
{
    for (int i = 0; i < board->width; i++)
    {
        for (int j = 0; j < board->height; j++)
        {
            tile_destroy(board->tiles[i + j * board->width]);

            if (board->pieces[i + j * board->width])
            {
                piece_destroy(board->pieces[i + j * board->width]);
            }
        }
    }
    SDL_free(board);
}



// ========== get ==========
int board_get_width(BoardState* board)
{
    return board->width;
}
int board_get_height(BoardState* board)
{
    return board->height;
}



// ========== pieces ==========
void board_add_piece_at(BoardState* board, Piece* piece, int x, int y)
{
    board->pieces[x + y * board->width] = piece;
}
void board_destroy_piece_at(BoardState* board, int x, int y)
{
    if (board->pieces[x + y * board->width])
    {
        piece_destroy(board->pieces[x + y * board->width]);
        board->pieces[x + y * board->width] = NULL;
    }
}
Piece* board_get_piece_at(BoardState* board, int x, int y)
{
    return board->pieces[x + y * board->width];
}

static void move_piece(BoardState* board, int src_x, int src_y, int dst_x, int dst_y)
{
    board->pieces[dst_x + dst_y * board->width] = board->pieces[src_x + src_y * board->width];
    board->pieces[src_x + src_y * board->width] = NULL;
}
void board_move_piece(BoardState* board, int src_x, int src_y, int dst_x, int dst_y)
{
    const Piece* src = board_get_piece_at(board, src_x, src_y);
    const Piece* dst = board_get_piece_at(board, dst_x, dst_y);
    if (!piece_exists(src))
    {
        move_piece(board, src_x, src_y, dst_x, dst_y);
        return;
    }
    else if (piece_can_capture(src, dst))
    {
        move_piece(board, src_x, src_y, dst_x, dst_y);
        return;
    }
}

bool board_can_move_piece(BoardState* board, int src_x, int src_y, int dst_x, int dst_y)
{
    const Piece* src = board_get_piece_at(board, src_x, src_y);
    const Piece* dst = board_get_piece_at(board, dst_x, dst_y);
    if (!piece_exists(src))
    {
        return true;
    }
    else if (piece_can_capture(src, dst))
    {
        return true;
    }
    return false;
}



// ========== tiles ==========
void board_increment_size(BoardState* board)
{
    // incrememt size
    board->width += 2;
    board->height += 2;

    // allocate more space
    board->tiles = SDL_realloc(board->tiles, sizeof(Tile*) * board->width * board->height);
    board->pieces = SDL_realloc(board->pieces, sizeof(Piece*) * board->width * board->height);

    // move pieces
    for (int i = board->width - 1; i >= 0; i--)
    {
        for (int j = board->height - 1; j >= 0; j--)
        {
            if (i > board->width - 1 - 2 || j > board->height - 1 - 2)
            {
                board->pieces[i + j * board->width] = NULL;
                continue;
            }
            move_piece(board, i, j, i + 1, j + 1);
        }
    }
}
