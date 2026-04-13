#include "../../../include/game/board/board.h"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>

typedef enum BoardTextures
{
    TEXTURE_WHITE_PAWN,
    TEXTURE_WHITE_ROOK,
    TEXTURE_WHITE_KNIGHT,
    TEXTURE_WHITE_BISHOP,
    TEXTURE_WHITE_QUEEN,
    TEXTURE_WHITE_KING,

    TEXTURE_BLACK_PAWN,
    TEXTURE_BLACK_ROOK,
    TEXTURE_BLACK_KNIGHT,
    TEXTURE_BLACK_BISHOP,
    TEXTURE_BLACK_QUEEN,
    TEXTURE_BLACK_KING,

    TEXTURE_TILE_WHITE,
    TEXTURE_TILE_BLACK,

    TEXTURE_TILE_SELECTED,
    TEXTURE_TILE_VALID,
    TEXTURE_TILE_CAPTURE,

    BOARD_TEXTURE_COUNT
} BoardTextures;

static const char* path[] =
{
    PATH_TEXTURE_WHITE_PAWN,
    PATH_TEXTURE_WHITE_ROOK,
    PATH_TEXTURE_WHITE_KNIGHT,
    PATH_TEXTURE_WHITE_BISHOP,
    PATH_TEXTURE_WHITE_QUEEN,
    PATH_TEXTURE_WHITE_KING,

    PATH_TEXTURE_BLACK_PAWN,
    PATH_TEXTURE_BLACK_ROOK,
    PATH_TEXTURE_BLACK_KNIGHT,
    PATH_TEXTURE_BLACK_BISHOP,
    PATH_TEXTURE_BLACK_QUEEN,
    PATH_TEXTURE_BLACK_KING,

    PATH_TEXTURE_TILE_WHITE,
    PATH_TEXTURE_TILE_BLACK,

    PATH_TEXTURE_TILE_SELECTED,
    PATH_TEXTURE_TILE_VALID,
    PATH_TEXTURE_TILE_CAPTURE,
};

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

static BoardTextures piece_get_texture_index(Piece* piece);
static BoardTextures tile_get_texture_index(Tile* tile, int col, int row);
static void board_add_tile(Board* board, Tile* tile, int col, int row);
static void board_remove_tile(Board* board, int col, int row);
static void board_remove_piece(Board* board, int col, int row);
static Piece* board_get_piece_at(Board* board, int col, int row);
static Tile* board_get_tile_at(Board* board, int col, int row);
static bool board_has_piece_at(Board* board, int col, int row);
static bool board_has_tile_at(Board* board, int col, int row);
static void board_piece_set_object(Board* board, int col, int row);
static void board_tile_set_object(Board* board, int col, int row);
static void board_tile_set_button(Board* board, int col, int row);
static void board_clear_tiles(Board* board);
static void board_set_selected_tile(void* arg1_board, void* arg2_tile);
static void board_set_valid_tiles(Board* board);
static int board_tile_get_col(Board* board, Tile* tile);
static int board_tile_get_row(Board* board, Tile* tile);

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
        NULL,
        NULL
    );
    button_set_size(
        button, 
        object_get_width(object),
        object_get_height(object)
    );
    button_set_on_press_func_arg2(button, board_set_selected_tile, board, tile);

    window_add_button(window, button);
    tile_set_button(tile, button);
}
static void board_clear_tiles(Board* board)
{
    verify(board == NULL, "Board does not exist");

    Tile* tile;
    for (int col = 0; col < board->col_num; col++)
    {
        for (int row = 0; row < board->row_num; row++)
        {
            tile = board_get_tile_at(board, col, row);
            tile_set_texture(tile, NULL);
        }
    }

}
static void board_set_selected_tile(void* arg1_board, void* arg2_tile)
{
    Board* board = (Board*)arg1_board;
    verify(board == NULL, "Board does not exist");

    Tile* new_tile = (Tile*)arg2_tile;
    Tile* old_tile = board->selected_tile;
    Button* button = NULL;

    board_clear_tiles(board);

    board->selected_tile = new_tile;

    if (board->selected_tile == NULL) return;

    button = tile_get_button(new_tile);

    BoardTextures texture_index = TEXTURE_TILE_SELECTED;
    SDL_Texture* texture = window_get_texture(
        board_get_window(board), 
        texture_index
    );

    button_set_texture_idle(button, texture);
    button_set_texture_hovered(button, texture);
    button_set_texture_pressed(button, texture);

    board_set_valid_tiles(board);


    
    if (old_tile == NULL) return;
    if (new_tile == NULL) return;
    int old_col = board_tile_get_col(board, old_tile);
    int old_row = board_tile_get_row(board, old_tile);
    int new_col = board_tile_get_col(board, new_tile);
    int new_row = board_tile_get_row(board, new_tile);

    if (!board_has_piece_at(board, old_col, old_row)) return;
    Piece* piece = board_get_piece_at(board, old_col, old_row);

    if(piece_get_type(piece) == KNIGHT &&
        can_knight_move_to(old_col, old_row, new_col, new_row))
    {
        board_move_piece_from_to(board, old_col, old_row, new_col, new_row);
    }
}
static void board_set_valid_tiles(Board* board)
{
    verify(board == NULL, "Board does not exist");
    if (board->selected_tile == NULL) return;

    int selected_col = board_tile_get_col(board, board->selected_tile);
    int selected_row = board_tile_get_row(board, board->selected_tile);

    if (!board_has_piece_at(board, selected_col, selected_row))
        return;
    Piece* piece = board_get_piece_at(board, selected_col, selected_row);
    PieceType type = piece_get_type(piece);

    SDL_Texture* texture = window_get_texture(board_get_window(board), TEXTURE_TILE_VALID);

    Tile* tile;
    Button* button;
    for (int col = 0; col < board->col_num; col++)
    {
        for (int row = 0; row < board->row_num; row++)
        {
            if(piece_get_type(piece) == KNIGHT &&
                can_knight_move_to(selected_col, selected_row, col, row))
            {
                tile = board_get_tile_at(board, col, row);
                button = tile_get_button(tile);
                tile_set_texture(tile, texture);
            }
        }
    }
}



static int board_piece_get_col(Board* board, Piece* piece)
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
static int board_piece_get_row(Board* board, Piece* piece)
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
static int board_tile_get_col(Board* board, Tile* tile)
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
static int board_tile_get_row(Board* board, Tile* tile)
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



static void board_remove_piece(Board* board, int col, int row)
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
        board_set_selected_tile(board, NULL);
    }

    tile_destroy(tile);
    board->tiles[col + row * board->col_num] = NULL;
}



static Piece* board_get_piece_at(Board* board, int col, int row)
{
    verify(board == NULL, "Board does not exist");
    verify(col < 0 || col > board->col_num, "Invalid board position");
    verify(row < 0 || row > board->row_num, "Invalid board position");

    return board->pieces[col + row * board->col_num];
}
static Tile* board_get_tile_at(Board* board, int col, int row)
{
    verify(board == NULL, "Board does not exist");
    verify(col < 0 || col > board->col_num, "Invalid board position");
    verify(row < 0 || row > board->row_num, "Invalid board position");

    return board->tiles[col + row * board->col_num];
}



static bool board_has_piece_at(Board* board, int col, int row)
{
    verify(board == NULL, "Board does not exist");
    verify(col < 0 || col >= board->col_num , "Invalid position");
    verify(row < 0 || row >= board->row_num , "Invalid position");

    return board_get_piece_at(board, col, row) != NULL;
}
static bool board_has_tile_at(Board* board, int col, int row)
{
    verify(board == NULL, "Board does not exist");
    verify(!is_within(col, 0, board_get_col_num(board) - 1), "Invalid position");
    verify(!is_within(row, 0, board_get_row_num(board) - 1), "Invalid position");

    return board_get_tile_at(board, col, row) != NULL;
}



void board_move_piece_from_to(Board* board, int src_col, int src_row, int dst_col, int dst_row)
{
    verify(board == NULL, "Board does not exist");
    verify(src_col < 0 || src_col >= board->col_num , "Invalid position");
    verify(src_row < 0 || src_row >= board->row_num , "Invalid position");
    verify(dst_col < 0 || dst_col >= board->col_num , "Invalid position");
    verify(dst_row < 0 || dst_row >= board->row_num , "Invalid position");
    verify(!board_has_piece_at(board, src_col, src_row), "Piece does not exist");
    verify(board_has_piece_at(board, dst_col, dst_row), "Destination already occupied");

    Piece* piece = board_get_piece_at(board, src_col, src_row);
    board->pieces[dst_col + dst_row * board->col_num] = board->pieces[src_col + src_row * board->col_num];
    board->pieces[src_col + src_row * board->col_num] = NULL;

    object_set_position(
        piece_get_object(piece), 
        window_get_x(board->window) + (dst_col * board->scale * TEXTURE_DEFAULT_SIZE_PX), 
        window_get_y(board->window) + (dst_row * board->scale * TEXTURE_DEFAULT_SIZE_PX)
    );

    board_clear_tiles(board);
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



void board_set_default_layout(Board* board)
{
    verify(board == NULL, "Board does not exist");

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
