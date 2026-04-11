#include "../../../include/game/board/board.h"

struct Board
{
    int col_num;
    int row_num;

    Tile** tiles;

    Piece** pieces;
};



// ========== create ==========
Board* board_create(int col_num, int row_num)
{
    verify(col_num <= 0 || row_num <= 0, "col_num and row_num must be positive");

    Board* board = (Board*)SDL_malloc(sizeof(Board));

    board->col_num = col_num;
    board->row_num = row_num;

    board->tiles = SDL_malloc(sizeof(Tile*) * board->col_num * board->row_num);
    board->pieces = SDL_malloc(sizeof(Piece*) * board->col_num * board->row_num);

    for (int i = 0; i < board->col_num; i++)
    {
        for (int j = 0; j < board->row_num; j++)
        {
            board->tiles[i + j * board->col_num] = tile_create(TILE_NONE);
            board->pieces[i + j * board->col_num] = NULL;
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

    for (int x = 0; x < board->col_num; x++)
    {
        for (int y = 0; y < board->row_num; y++)
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



// ========== window ==========
static BoardTextures piece_get_texture(Piece* piece)
{
    verify(piece == NULL, "Piece does not exist");

    PieceColor color = piece_get_color(piece);

    switch (piece_get_type(piece))
    {
        case PAWN:
            if (color == PIECE_WHITE)
                return TEXTURE_WHITE_PAWN;
            else
                return TEXTURE_BLACK_PAWN;
            break;
        case ROOK:
            if (color == PIECE_WHITE)
                return TEXTURE_WHITE_ROOK;
            else
                return TEXTURE_BLACK_ROOK;
            break;
        case KNIGHT:
            if (color == PIECE_WHITE)
                return TEXTURE_WHITE_KNIGHT;
            else
                return TEXTURE_BLACK_KNIGHT;
            break;
        case BISHOP:
            if (color == PIECE_WHITE)
                return TEXTURE_WHITE_BISHOP;
            else
                return TEXTURE_BLACK_BISHOP;
            break;
        case QUEEN:
            if (color == PIECE_WHITE)
                return TEXTURE_WHITE_QUEEN;
            else
                return TEXTURE_BLACK_QUEEN;
            break;
        case KING:
            if (color == PIECE_WHITE)
                return TEXTURE_WHITE_KING;
            else
                return TEXTURE_BLACK_KING;
            break;
        default:
            return 0;
            break;
    }
}
static BoardTextures tile_get_texture(Tile* tile, int x, int y)
{
    verify(tile == NULL, "TILE does not exist");

    if (is_even(x + y))
        return TEXTURE_TILE_BLACK;
    else
        return TEXTURE_TILE_WHITE;

}
static void board_window_add_tiles(Window* window, Board* board)
{
    verify(window == NULL, "Window does not exist");
    verify(board == NULL, "Board does not exist");

    SDL_Texture* texture = NULL;
    Tile* tile;
    int screen_pos_x;
    int screen_pos_y;

    for (int x = 0; x < board_get_col_num(board); x++)
    {
        for (int y = 0; y < board_get_row_num(board); y++)
        {
            tile = board_get_tile_at(board, x, y);
            texture = window_get_texture(window, tile_get_texture(tile, x, y));

            screen_pos_x = x * 64;
            screen_pos_y = y * 64;

            Object* object = object_create(
                screen_pos_x, 
                screen_pos_y, 
                texture
            );
            object_set_size(object, 64, 64);
            window_add_object(window, object);
        }
    }
}
static void board_window_add_pieces(Window* window, Board* board)
{
    verify(window == NULL, "Window does not exist");
    verify(board == NULL, "Board does not exist");

    SDL_Texture* texture = NULL;
    Piece* piece;
    int screen_pos_x;
    int screen_pos_y;

    for (int x = 0; x < board_get_col_num(board); x++)
    {
        for (int y = 0; y < board_get_row_num(board); y++)
        {
            piece = board_get_piece_at(board, x, y);
            if (piece == NULL) continue;

            texture = window_get_texture(window, piece_get_texture(piece));

            screen_pos_x = x * 64;
            screen_pos_y = y * 64;

            Object* object = object_create(
                screen_pos_x, 
                screen_pos_y, 
                texture
            );
            object_set_size(object, 64, 64);
            window_add_object(window, object);
        }
    }
}
Window* board_create_window(SDL_Renderer* renderer, Board* board, double x, double y, double width, double height)
{
    verify(renderer == NULL, "SDL_Renderer does not exist");
    verify(board == NULL, "Board does not exist");
    verify(width < 0 || height < 0, "Invalid size");

    const char* path[] =
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
    };

    Window* window = window_create(
        x, 
        y, 
        width, 
        height, 
        SDL_CreateTextureFromPNG(
            renderer,
            PATH_TEXTURE_WINDOW_BOARD_BACKGROUND
        )
    );

    window_load_textures(renderer, window, path, BOARD_TEXTURE_COUNT);

    board_window_add_tiles(window, board);
    board_window_add_pieces(window, board);

    return window;
}



// ========== get ==========
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



// ========== pieces ==========
static void board_set_piece_at(Board* board, Piece* piece, int x, int y)
{
    verify(board == NULL, "Board does not exist");
    verify(piece == NULL, "Piece does not exist");
    verify(!is_within(x, 0, board_get_col_num(board) - 1), "Invalid position");
    verify(!is_within(y, 0, board_get_row_num(board) - 1), "Invalid position");

    board->pieces[x + y * board->col_num] = piece;
}

void board_add_piece_at(Board* board, Piece* piece, int x, int y)
{
    verify(board == NULL, "Board does not exist");
    verify(piece == NULL, "Piece does not exist");
    verify(!is_within(x, 0, board_get_col_num(board) - 1), "Invalid position");
    verify(!is_within(y, 0, board_get_row_num(board) - 1), "Invalid position");
    verify(board_has_piece_at(board, x, y), "Piece already at position, could not add");

    board_set_piece_at(board, piece, x, y);
}

static Piece* board_pop_piece_at(Board* board, int x, int y)
{
    verify(board == NULL, "Board does not exist");
    verify(!is_within(x, 0, board_get_col_num(board) - 1), "Invalid position");
    verify(!is_within(y, 0, board_get_row_num(board) - 1), "Invalid position");
    verify(!board_has_piece_at(board, x, y), "No piece at position, could not pop");

    Piece* piece = board_get_piece_at(board, x, y);
    board->pieces[x + y * board->col_num] = NULL;
    return piece;
}

void board_destroy_piece_at(Board* board, int x, int y)
{
    verify(board == NULL, "Board does not exist");
    verify(!is_within(x, 0, board_get_col_num(board) - 1), "Invalid position");
    verify(!is_within(y, 0, board_get_row_num(board) - 1), "Invalid position");
    verify(!board_has_piece_at(board, x, y), "Piece does not exist, could not be destroyed");

    piece_destroy(board_pop_piece_at(board, x, y));
}

Piece* board_get_piece_at(Board* board, int x, int y)
{
    verify(board == NULL, "Board does not exist");
    verify(!is_within(x, 0, board_get_col_num(board) - 1), "Invalid position");
    verify(!is_within(y, 0, board_get_row_num(board) - 1), "Invalid position");

    return board->pieces[x + y * board->col_num];
}

bool board_has_piece_at(Board* board, int x, int y)
{
    verify(board == NULL, "Board does not exist");
    verify(!is_within(x, 0, board_get_col_num(board) - 1), "Invalid position");
    verify(!is_within(y, 0, board_get_row_num(board) - 1), "Invalid position");

    return board_get_piece_at(board, x, y) != NULL;
}

void board_move_piece(Board* board, int src_x, int src_y, int dst_x, int dst_y)
{
    verify(board == NULL, "Board does not exist");
    verify(!is_within(src_x, 0, board_get_col_num(board) - 1), "Invalid position");
    verify(!is_within(src_y, 0, board_get_row_num(board) - 1), "Invalid position");
    verify(!is_within(dst_x, 0, board_get_col_num(board) - 1), "Invalid position");
    verify(!is_within(dst_y, 0, board_get_row_num(board) - 1), "Invalid position");
    verify(board_has_piece_at(board, dst_x, dst_y), "Destination position already occupied");

    Piece* piece = board_pop_piece_at(board, src_x, src_y);
    board_set_piece_at(board, piece, dst_x, dst_y);
}



// ========== tiles ==========
Tile* board_get_tile_at(const Board* board, int x, int y)
{
    verify(board == NULL, "Board does not exist");
    verify(!is_within(x, 0, board_get_col_num(board) - 1), "Invalid position");
    verify(!is_within(y, 0, board_get_row_num(board) - 1), "Invalid position");

    return board->tiles[x + y * board->col_num];
}

void board_expand(Board* board)
{
    // TODO: clean mess

    verify(board == NULL, "Board does not exist");

    // incrememt size
    board->col_num += 2;
    board->row_num += 2;

    // allocate more space
    board->tiles = SDL_realloc(board->tiles, sizeof(Tile*) * board->col_num * board->row_num);
    board->pieces = SDL_realloc(board->pieces, sizeof(Piece*) * board->col_num * board->row_num);

    // move pieces
    for (int i = board->col_num - 1; i >= 0; i--)
    {
        for (int j = board->row_num - 1; j >= 0; j--)
        {
            if (i > board->col_num - 1 - 2 || j > board->row_num - 1 - 2)
            {
                board->pieces[i + j * board->col_num] = NULL;
                continue;
            }
            board_move_piece(board, i, j, i + 1, j + 1);
        }
    }
}


