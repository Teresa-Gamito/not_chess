#include "../../../include/game/board/board.h"
#include <SDL3/SDL_stdinc.h>

struct Board
{
    int col_num;
    int row_num;

    Window* window;
    double scale;
    Tile* selected_tile;

    Tile** tiles;
    Piece** pieces;
};


static void board_add_tile(Board* board, Tile* tile, int col, int row);
static void board_remove_tile(Board* board, int col, int row);

static bool board_piece_has_clear_path(Board* board, int src_col, int src_row, int dst_col, int dst_row);

static void board_select_tile(void* arg1_board, void* arg2_tile);

static void board_ui_update(Board* board);
static void board_tile_set_ui(Board* board, Tile* tile);
static void board_tile_set_button(Board* board, int col, int row);

static void board_piece_set_object(Board* board, int col, int row);
static BoardTextures piece_get_texture_index(Piece* piece);

static void board_tile_set_object(Board* board, int col, int row);
static BoardTextures tile_get_texture_index(Tile* tile, int col, int row);




Board* board_create(
    SDL_Renderer* renderer, 
    int col_num, 
    int row_num, 
    double window_x, 
    double window_y, 
    double window_width, 
    double window_height
)
{
    Board* board = SDL_malloc(sizeof(Board));
    verify(board == NULL, "Board could not be created: malloc");

    board->col_num = col_num;
    board->row_num = row_num;

    if ((double) col_num / row_num > window_width / window_height)
        board->scale = window_width / (col_num * TEXTURE_DEFAULT_SIZE_PX);
    else 
        board->scale = window_height / (row_num * TEXTURE_DEFAULT_SIZE_PX);

    board->window = window_create(
        window_x,
        window_y,
        window_width,
        window_height,
        SDL_CreateTextureFromPNG(
            renderer,
            PATH_TEXTURE_WINDOW_BOARD_BACKGROUND
        )
    );
    window_load_textures(renderer, board->window, path, BOARD_TEXTURE_COUNT);

    board->selected_tile = NULL;

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
            board_add_tile(board, tile, col, row);
        }
    }

    return board;
}

void board_destroy(Board* board)
{
    verify(board == NULL, "Board does not exist");

    window_destroy(board->window);

    int col_num = board->col_num;
    int row_num = board->row_num;

    for (int col = 0; col < col_num; col++)
    {
        for (int row = 0; row < row_num; row++)
        {
            tile_destroy(board_get_tile_at(board, col, row));

            if (!board_has_piece_at(board, col, row))
            {
                continue;
            }
            piece_destroy(board_get_piece_at(board, col, row));
        }
    }
    SDL_free(board->tiles);
    SDL_free(board->pieces);

    SDL_free(board);
}



void board_render(SDL_Renderer* renderer, const Board* board)
{
    window_render(renderer, board_get_window(board));
}

void board_update(InputState* input, Board* board)
{
    window_update(input, board_get_window(board));
}



void board_add_piece_at(Board* board, Piece* piece, int col, int row)
{
    verify(board == NULL, "Board does not exist");
    verify(piece == NULL, "Piece does not exist");
    verify(col < 0 || col > board->col_num, "Invalid board position");
    verify(row < 0 || row > board->row_num, "Invalid board position");
    verify(board_has_piece_at(board, col, row), "Could not add piece, position already occupied");

    board->pieces[col + row * board->col_num] = piece;

    board_piece_set_object(board, col, row);
}
void board_remove_piece(Board* board, int col, int row)
{
    verify(board == NULL, "Board does not exist");
    verify(col < 0 || col > board->col_num, "Invalid board position");
    verify(row < 0 || row > board->row_num, "Invalid board position");
    verify(!board_has_piece_at(board, col, row), "Piece does not exist");

    Piece* piece = board_get_piece_at(board, col, row);

    Window* window = board_get_window(board);
    Object* object = piece_get_object(piece);
    window_destroy_object(window, object);

    piece_destroy(piece);
    board->pieces[col + row * board->col_num] = NULL;
}
bool board_has_piece_at(Board* board, int col, int row)
{
    verify(board == NULL, "Board does not exist");
    verify(col < 0 || col >= board->col_num , "Invalid position");
    verify(row < 0 || row >= board->row_num , "Invalid position");

    return board_get_piece_at(board, col, row) != NULL;
}
Piece* board_get_piece_at(Board* board, int col, int row)
{
    verify(board == NULL, "Board does not exist");
    verify(col < 0 || col > board->col_num, "Invalid board position");
    verify(row < 0 || row > board->row_num, "Invalid board position");

    return board->pieces[col + row * board->col_num];
}
int board_piece_get_col(Board* board, Piece* piece)
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
int board_piece_get_row(Board* board, Piece* piece)
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
void board_piece_move_to(Board* board, int src_col, int src_row, int dst_col, int dst_row)
{
    verify(board == NULL, "Board does not exist");
    verify(src_col < 0 || src_col >= board->col_num , "Invalid position");
    verify(src_row < 0 || src_row >= board->row_num , "Invalid position");
    verify(dst_col < 0 || dst_col >= board->col_num , "Invalid position");
    verify(dst_row < 0 || dst_row >= board->row_num , "Invalid position");
    verify(!board_has_piece_at(board, src_col, src_row), "Piece does not exist");

    if (!board_piece_has_clear_path(board, src_col, src_row, dst_col, dst_row))
    {
        return;
    }

    Piece* piece = board_get_piece_at(board, src_col, src_row);
    board->pieces[dst_col + dst_row * board->col_num] = board->pieces[src_col + src_row * board->col_num];
    board->pieces[src_col + src_row * board->col_num] = NULL;

    object_set_position(
        piece_get_object(piece), 
        window_get_x(board->window) + (dst_col * board->scale * TEXTURE_DEFAULT_SIZE_PX), 
        window_get_y(board->window) + (dst_row * board->scale * TEXTURE_DEFAULT_SIZE_PX)
    );

    board->selected_tile = NULL;
    board_ui_update(board);
}
static bool board_piece_has_clear_path(Board* board, int src_col, int src_row, int dst_col, int dst_row)
{
    verify(board == NULL, "Board does not exist");
    verify(src_col < 0 || src_col >= board->col_num , "Invalid position");
    verify(src_row < 0 || src_row >= board->row_num , "Invalid position");
    verify(dst_col < 0 || dst_col >= board->col_num , "Invalid position");
    verify(dst_row < 0 || dst_row >= board->row_num , "Invalid position");
    verify(!board_has_piece_at(board, src_col, src_row), "Piece does not exist");

    Piece* piece = board_get_piece_at(board, src_col, src_row);
    verify(!piece_can_move_to(piece, src_col, src_row, dst_col, dst_row), "Piece cant move to location");

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



static void board_add_tile(Board* board, Tile* tile, int col, int row)
{
    verify(board == NULL, "Board does not exist");
    verify(tile == NULL, "Tile does not exist");
    verify(col < 0 || col > board->col_num, "Invalid board position");
    verify(row < 0 || row > board->row_num, "Invalid board position");
    verify(board_has_tile_at(board, col, row), "Could not add tile, position already occupied");

    board->tiles[col + row * board->col_num] = tile;

    board_tile_set_object(board, col, row);
    board_tile_set_button(board, col, row);
}
static void board_remove_tile(Board* board, int col, int row)
{
    verify(board == NULL, "Board does not exist");
    verify(col < 0 || col > board->col_num, "Invalid board position");
    verify(row < 0 || row > board->row_num, "Invalid board position");
    verify(!board_has_tile_at(board, col, row), "Tile does not exist");

    Tile* tile = board_get_tile_at(board, col, row);

    Window* window = board_get_window(board);
    Object* object = tile_get_object(tile);
    window_destroy_object(window, object);
    Button* button = tile_get_button(tile);
    window_destroy_button(window, button);

    if (board->selected_tile == tile)
    {
        board->selected_tile = NULL;
    }

    tile_destroy(tile);
    board->tiles[col + row * board->col_num] = NULL;
}
bool board_has_tile_at(Board* board, int col, int row)
{
    verify(board == NULL, "Board does not exist");
    verify(!is_within(col, 0, board_get_col_num(board) - 1), "Invalid position");
    verify(!is_within(row, 0, board_get_row_num(board) - 1), "Invalid position");

    return board_get_tile_at(board, col, row) != NULL;
}
Tile* board_get_tile_at(Board* board, int col, int row)
{
    verify(board == NULL, "Board does not exist");
    verify(col < 0 || col > board->col_num, "Invalid board position");
    verify(row < 0 || row > board->row_num, "Invalid board position");

    return board->tiles[col + row * board->col_num];
}
int board_tile_get_col(Board* board, Tile* tile)
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
int board_tile_get_row(Board* board, Tile* tile)
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
Window* board_get_window(const Board* board)
{
    verify(board == NULL, "Board does not exist");

    return board->window;
}



// ==============================
// Board UI
// ==============================

void board_select_tile(void* arg1_board, void* arg2_tile)
{
    Board* board = (Board*)arg1_board;
    verify(board == NULL, "Board does not exist");

    Tile* new_tile = (Tile*)arg2_tile;
    Tile* old_tile = board->selected_tile;

        board_ui_update(board);
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
    if (!board_has_piece_at(board, old_col, old_row)) 
    {
        return;
    }
    Piece* src_piece = board_get_piece_at(board, old_col, old_row);

    int new_col = board_tile_get_col(board, new_tile);
    int new_row = board_tile_get_row(board, new_tile);
    Piece* dst_piece = board_get_piece_at(board, old_col, old_row);

    if (piece_can_move_to(src_piece, old_col, old_row, new_col, new_row))
    {
        board_piece_move_to(board, old_col, old_row, new_col, new_row);
    }
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
            tile_set_ui(tile, texture_selected, texture_valid, texture_capture);
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

    Piece* src_piece = board_get_piece_at(board, src_col, src_row);
    if (!board_has_piece_at(board, dst_col, dst_row))
    {
        if (piece_can_move_to(src_piece, src_col, src_row, dst_col, dst_row) &&
            board_piece_has_clear_path(board, src_col, src_row, dst_col, dst_row))
        {
            tile_ui_set_valid(tile);
            return;
        }
    }

    Piece* dst_piece = board_get_piece_at(board, src_col, src_row);
    if (piece_can_capture(src_piece, dst_piece, src_col, src_row, dst_col, dst_row))
    {
        tile_ui_set_capture(tile);
        return;
    }

    tile_ui_set_none(tile);
}

static void board_piece_set_object(Board* board, int col, int row)
{
    verify(board == NULL, "Board does not exist");
    verify(col < 0 || col > board->col_num, "Invalid board position");
    verify(row < 0 || row > board->row_num, "Invalid board position");
    verify(!board_has_piece_at(board, col, row), "Piece does not exist");

    Piece* piece = board_get_piece_at(board, col, row);
    Window* window = board_get_window(board);

    BoardTextures index = piece_get_texture_index(piece);
    SDL_Texture* texture = window_get_texture(window, index);

    // TODO: center board
    Object* object = object_create(
        window_get_x(window) + (col * board->scale * TEXTURE_DEFAULT_SIZE_PX), 
        window_get_y(window) + (row * board->scale * TEXTURE_DEFAULT_SIZE_PX), 
        texture
    );
    object_set_size(
        object, 
        board->scale * TEXTURE_DEFAULT_SIZE_PX, 
        board->scale * TEXTURE_DEFAULT_SIZE_PX
    );

    window_add_object(window, object);
    piece_set_object(piece, object);
}
static void board_tile_set_object(Board* board, int col, int row)
{
    verify(board == NULL, "Board does not exist");
    verify(col < 0 || col > board->col_num, "Invalid board position");
    verify(row < 0 || row > board->row_num, "Invalid board position");
    verify(!board_has_tile_at(board, col, row), "Piece does not exist");

    Tile* tile = board_get_tile_at(board, col, row);
    Window* window = board_get_window(board);

    BoardTextures index = tile_get_texture_index(tile, col, row);
    SDL_Texture* texture = window_get_texture(window, index);

    // TODO: center board
    Object* object = object_create(
        window_get_x(window) + (col * board->scale * TEXTURE_DEFAULT_SIZE_PX), 
        window_get_y(window) + (row * board->scale * TEXTURE_DEFAULT_SIZE_PX), 
        texture
    );
    object_set_size(
        object, 
        board->scale * TEXTURE_DEFAULT_SIZE_PX, 
        board->scale * TEXTURE_DEFAULT_SIZE_PX
    );

    window_add_object(window, object);
    tile_set_object(tile, object);
}
static void board_tile_set_button(Board* board, int col, int row)
{
    verify(board == NULL, "Board does not exist");
    verify(col < 0 || col > board->col_num, "Invalid board position");
    verify(row < 0 || row > board->row_num, "Invalid board position");
    verify(!board_has_tile_at(board, col, row), "Piece does not exist");

    Tile* tile = board_get_tile_at(board, col, row);
    Window* window = board_get_window(board);
    Object* object = tile_get_object(tile);

    
    Button* button = button_create(
        object_get_x(object),
        object_get_y(object),
        NULL,
        window_get_texture(window, TEXTURE_TILE_SELECTED),
        NULL
    );
    button_set_size(
        button, 
        object_get_width(object),
        object_get_height(object)
    );
    button_set_on_press_func_arg2(button, board_select_tile, board, tile);

    window_add_button(window, button);
    tile_set_button(tile, button);
}





static BoardTextures piece_get_texture_index(Piece* piece)
{
    verify(piece == NULL, "Piece does not exist");

    PieceColor color = piece_get_color(piece);
    PieceType type = piece_get_type(piece);

    if (color == PIECE_WHITE)
    {
        switch (type)
        {
            case PAWN:
                return TEXTURE_WHITE_PAWN;
                break;
            case ROOK:
                return TEXTURE_WHITE_ROOK;
                break;
            case KNIGHT:
                return TEXTURE_WHITE_KNIGHT;
                break;
            case BISHOP:
                return TEXTURE_WHITE_BISHOP;
                break;
            case QUEEN:
                return TEXTURE_WHITE_QUEEN;
                break;
            case KING:
                return TEXTURE_WHITE_KING;
                break;
            default:
                return 0;
                break;
        }
    }
    else if (color == PIECE_BLACK)
    {
        switch (type)
        {
            case PAWN:
                return TEXTURE_BLACK_PAWN;
                break;
            case ROOK:
                return TEXTURE_BLACK_ROOK;
                break;
            case KNIGHT:
                return TEXTURE_BLACK_KNIGHT;
                break;
            case BISHOP:
                return TEXTURE_BLACK_BISHOP;
                break;
            case QUEEN:
                return TEXTURE_BLACK_QUEEN;
                break;
            case KING:
                return TEXTURE_BLACK_KING;
                break;
            default:
                return 0;
                break;
        }
    }
    return 0;
}
static BoardTextures tile_get_texture_index(Tile* tile, int x, int y)
{
    verify(tile == NULL, "Tile does not exist");

    if (is_even(x + y))
        return TEXTURE_TILE_BLACK;
    else
        return TEXTURE_TILE_WHITE;
}
