#include "../../../include/game/board/board.h"

struct Board
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
Board* board_create(int width, int height)
{
    verify(width <= 0 || height <= 0, "Width and height must be positive");

    Board* board = (Board*)SDL_malloc(sizeof(Board));

    board->width = width;
    board->height = height;

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

    return board;
}

void board_set_default(Board* board)
{
    verify(board == NULL, "Board does not exist");

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

    board_add_piece_at(board, piece_create(KING, PIECE_BLACK), 4, 7);
    board_add_piece_at(board, piece_create(QUEEN, PIECE_BLACK), 3, 7);
    board_add_piece_at(board, piece_create(BISHOP, PIECE_BLACK), 2, 7);
    board_add_piece_at(board, piece_create(BISHOP, PIECE_BLACK), 5, 7);
    board_add_piece_at(board, piece_create(KNIGHT, PIECE_BLACK), 1, 7);
    board_add_piece_at(board, piece_create(KNIGHT, PIECE_BLACK), 6, 7);
    board_add_piece_at(board, piece_create(ROOK, PIECE_BLACK), 0, 7);
    board_add_piece_at(board, piece_create(ROOK, PIECE_BLACK), 7, 7);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 0, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 1, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 2, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 3, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 4, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 5, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 6, 6);
    board_add_piece_at(board, piece_create(PAWN, PIECE_BLACK), 7, 6);
}



// ========== destroy ==========
void board_destroy(Board* board)
{
    verify(board == NULL, "Board does not exist");

    for (int x = 0; x < board->width; x++)
    {
        for (int y = 0; y < board->height; y++)
        {
            tile_destroy(board_get_tile_at(board, x, y));

            if (board_has_piece_at(board, x, y))
            {
                piece_destroy(board_get_piece_at(board, x, y));
            }
        }
    }
    SDL_free(board);
}



// ========== get ==========
int board_get_width(const Board* board)
{
    verify(board == NULL, "Board does not exist");

    return board->width;
}
int board_get_height(const Board* board)
{
    verify(board == NULL, "Board does not exist");

    return board->height;
}



// ========== pieces ==========
static void board_set_piece_at(Board* board, Piece* piece, int x, int y)
{
    verify(board == NULL, "Board does not exist");
    verify(piece == NULL, "Piece does not exist");
    verify(!is_within(x, 0, board_get_width(board) - 1), "Invalid position");
    verify(!is_within(y, 0, board_get_height(board) - 1), "Invalid position");

    board->pieces[x + y * board->width] = piece;
}

void board_add_piece_at(Board* board, Piece* piece, int x, int y)
{
    verify(board == NULL, "Board does not exist");
    verify(piece == NULL, "Piece does not exist");
    verify(!is_within(x, 0, board_get_width(board) - 1), "Invalid position");
    verify(!is_within(y, 0, board_get_height(board) - 1), "Invalid position");
    verify(board_has_piece_at(board, x, y), "Piece already at position, could not add");

    board_set_piece_at(board, piece, x, y);
}

static Piece* board_pop_piece_at(Board* board, int x, int y)
{
    verify(board == NULL, "Board does not exist");
    verify(!is_within(x, 0, board_get_width(board) - 1), "Invalid position");
    verify(!is_within(y, 0, board_get_height(board) - 1), "Invalid position");
    verify(!board_has_piece_at(board, x, y), "No piece at position, could not pop");

    Piece* piece = board_get_piece_at(board, x, y);
    board->pieces[x + y * board->width] = NULL;
    return piece;
}

void board_destroy_piece_at(Board* board, int x, int y)
{
    verify(board == NULL, "Board does not exist");
    verify(!is_within(x, 0, board_get_width(board) - 1), "Invalid position");
    verify(!is_within(y, 0, board_get_height(board) - 1), "Invalid position");
    verify(!board_has_piece_at(board, x, y), "Piece does not exist, could not be destroyed");

    piece_destroy(board_pop_piece_at(board, x, y));
}

Piece* board_get_piece_at(Board* board, int x, int y)
{
    verify(board == NULL, "Board does not exist");
    verify(!is_within(x, 0, board_get_width(board) - 1), "Invalid position");
    verify(!is_within(y, 0, board_get_height(board) - 1), "Invalid position");

    return board->pieces[x + y * board->width];
}

bool board_has_piece_at(Board* board, int x, int y)
{
    verify(board == NULL, "Board does not exist");
    verify(!is_within(x, 0, board_get_width(board) - 1), "Invalid position");
    verify(!is_within(y, 0, board_get_height(board) - 1), "Invalid position");

    return board_get_piece_at(board, x, y) != NULL;
}

void board_move_piece(Board* board, int src_x, int src_y, int dst_x, int dst_y)
{
    verify(board == NULL, "Board does not exist");
    verify(!is_within(src_x, 0, board_get_width(board) - 1), "Invalid position");
    verify(!is_within(src_y, 0, board_get_height(board) - 1), "Invalid position");
    verify(!is_within(dst_x, 0, board_get_width(board) - 1), "Invalid position");
    verify(!is_within(dst_y, 0, board_get_height(board) - 1), "Invalid position");
    verify(board_has_piece_at(board, dst_x, dst_y), "Destination position already occupied");

    Piece* piece = board_pop_piece_at(board, src_x, src_y);
    board_set_piece_at(board, piece, dst_x, dst_y);
}



// ========== tiles ==========
Tile* board_get_tile_at(const Board* board, int x, int y)
{
    verify(board == NULL, "Board does not exist");
    verify(!is_within(x, 0, board_get_width(board) - 1), "Invalid position");
    verify(!is_within(y, 0, board_get_height(board) - 1), "Invalid position");

    return board->tiles[x + y * board->width];
}

void board_expand(Board* board)
{
    // TODO: clean mess

    verify(board == NULL, "Board does not exist");

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
            board_move_piece(board, i, j, i + 1, j + 1);
        }
    }
}


