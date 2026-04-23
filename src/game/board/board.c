#include "include/game/board/board.h"

static bool board_piece_has_clear_path(const Board* board, int src_col, int src_row, int dst_col, int dst_row);

struct Board
{
    int col_num;
    int row_num;

    Tile** tiles;
    Piece** pieces;
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

    SDL_free(board);
}



bool board_can_add_piece_at(Board* board, int col, int row)
{
    verify(board == NULL, "Board does not exist");
    verify(col < 0 || col > board->col_num, "Invalid board position");
    verify(row < 0 || row > board->row_num, "Invalid board position");

    return !board_has_piece_at(board, col, row);
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
void board_piece_capture(Board* board, int src_col, int src_row, int dst_row, int dst_col)
{

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



// ==============================
// Board UI
// ==============================
/*
void board_select_tile(void* arg1_board, void* arg2_tile)
{
    Board* board = (Board*)arg1_board;
    verify(board == NULL, "Board does not exist");

    Tile* new_tile = (Tile*)arg2_tile;
    Tile* old_tile = board->selected_tile;

    if (new_tile == old_tile)
    {
        board->selected_tile = NULL;
        board_ui_update(board);
        return;
    }

    board->selected_tile = new_tile;
    tile_ui_set_selected(board->selected_tile);
    board_ui_update(board);

    if (new_tile == NULL) return;
    if (old_tile == NULL) return;

    int old_col = board_tile_get_col(board, old_tile);
    int old_row = board_tile_get_row(board, old_tile);
    int new_col = board_tile_get_col(board, new_tile);
    int new_row = board_tile_get_row(board, new_tile);

    if (!board_has_piece_at(board, old_col, old_row)) 
    {
        return;
    }
    if (board_piece_can_move_to(board, old_col, old_row, new_col, new_row))
    {
        board_piece_move_to(board, old_col, old_row, new_col, new_row);
        return;
    }

    if (!board_has_piece_at(board, new_col, new_row))
    {
        return;
    }
    if (board_piece_can_capture(board, old_col, old_row, new_col, new_row))
    {
        board_piece_capture(board, new_col, new_row);
        board_piece_move_to(board, old_col, old_row, new_col, new_row);
    }
}
void board_piece_capture(Board* board, int col, int row)
{
    // TODO: add points
    board_piece_remove(board, col, row);
}
static void board_ui_update(Board* board)
{
    verify(board == NULL, "Board does not exist");

    Window* window = board_get_window(board);

    BoardTextures texture_index_selected = TEXTURE_TILE_SELECTED;
    BoardTextures texture_index_valid = TEXTURE_TILE_VALID;
    BoardTextures texture_index_capture = TEXTURE_TILE_CAPTURE;

    SDL_Texture* texture_selected = window_get_texture(window, texture_index_selected);
    SDL_Texture* texture_valid = window_get_texture(window, texture_index_valid);
    SDL_Texture* texture_capture = window_get_texture(window, texture_index_capture);

    Tile* tile;
    for (int col = 0; col < board->col_num; col++)
    {
        for (int row = 0; row < board->row_num; row++)
        {
            tile = board_get_tile_at(board, col, row);
            board_tile_set_ui(board, tile);
            tile_set_ui_textures(tile, texture_selected, texture_valid, texture_capture);
        }
    }
}
static void board_tile_set_ui(Board* board, Tile* tile)
{
    if (board->selected_tile == NULL)
    {
        tile_ui_set_none(tile);
        return;
    }
    if (board->selected_tile == tile)
    {
        return;
    }

    int src_col = board_tile_get_col(board, board->selected_tile);
    int src_row = board_tile_get_row(board, board->selected_tile);
    int dst_col = board_tile_get_col(board, tile);
    int dst_row = board_tile_get_row(board, tile);

    if (!board_has_piece_at(board, src_col, src_row))
    {
        tile_ui_set_none(tile);
        return;
    }

    if (board_has_piece_at(board, dst_col, dst_row))
    {
        if (board_piece_can_capture(board, src_col, src_row, dst_col, dst_row))
        {
            tile_ui_set_capture(tile);
            return;
        }
    }
    if (board_piece_can_move_to(board, src_col, src_row, dst_col, dst_row))
    {
        tile_ui_set_valid(tile);
        return;
    }
    tile_ui_set_none(tile);
}
*/
