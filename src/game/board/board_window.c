#include "include/game/board/board_window.h"
#include "game/board/board.h"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>

static void board_ui_add_tile(BoardUI* ui, int col, int row);
static void board_ui_add_piece(BoardUI* ui, int col, int row);
static BoardTextures tile_get_texture_index(int col, int row);
static BoardTextures piece_get_texture_index(const Piece* piece);

struct BoardUI
{
    Board* board;
    Window* window;
    Tile* selected_tile;
    Vector* tasks;
};

BoardUI* board_ui_create(SDL_Renderer* renderer, Board* board, float x, float y, float width, float height)
{
    BoardUI* ui = SDL_malloc(sizeof(BoardUI));
    verify(ui == NULL, "BoardUI could not be created: malloc");

    ui->board = board;

    SDL_Texture* background_texture = SDL_CreateTextureFromPNG(renderer, PATH_TEXTURE_WINDOW_BOARD_BACKGROUND);
    ui->window = window_create(x, y, width, height, background_texture);
    window_load_textures(renderer, ui->window, board_textures, BOARD_TEXTURE_COUNT);

    ui->selected_tile = NULL;

    ui->tasks = vector_create(default_ops());

    int col_num = board_get_col_num(board);
    int row_num = board_get_row_num(board);

    if ((double) col_num / row_num > width / height)
    {
        window_set_scale(ui->window, width / (col_num * TEXTURE_DEFAULT_SIZE_PX));
    }
    else
    {
        window_set_scale(ui->window, height / (row_num * TEXTURE_DEFAULT_SIZE_PX));
    }

    board_ui_update(ui);

    return ui;
}
void board_ui_destroy(BoardUI* ui)
{
    window_destroy(ui->window);
    vector_destroy(ui->tasks);
    SDL_free(ui);
}
static SDL_Texture* board_ui_tile_get_texture(BoardUI* ui, int new_col, int new_row)
{
    verify(ui == NULL, "BoardUI does not exist");
    verify(new_col < 0 || new_col > board_get_col_num(ui->board), "Invalid position");
    verify(new_row < 0 || new_row > board_get_row_num(ui->board), "Invalid position");

    if (ui->selected_tile == NULL) return NULL;

    Board* board = ui->board;
    Tile* tile = board_get_tile_at(ui->board, new_col, new_row);
    int old_col = board_tile_get_col(board, ui->selected_tile);
    int old_row = board_tile_get_row(board, ui->selected_tile);

    if (tile == ui->selected_tile)
    {
        return window_get_texture(ui->window, TEXTURE_TILE_SELECTED);
    }

    if (!board_has_piece_at(ui->board, old_col, old_row)) 
    {
        return NULL;
    }

    if (board_can_piece_move_to(board, old_col, old_row, new_col, new_row))
    {
        return window_get_texture(ui->window, TEXTURE_TILE_VALID);
    }

    if (!board_has_piece_at(board, new_col, new_row))
    {
        return NULL;
    }
    if (board_can_piece_capture(board, old_col, old_row, new_col, new_row))
    {
        return window_get_texture(ui->window, TEXTURE_TILE_CAPTURE);
    }
    return NULL;
}
static void board_ui_add_tile(BoardUI* ui, int col, int row)
{
    verify(ui == NULL, "BoardUI does not exist");
    verify(col < 0 || col > board_get_col_num(ui->board), "Invalid position");
    verify(row < 0 || row > board_get_row_num(ui->board), "Invalid position");

    Window* window = ui->window;

    int index = tile_get_texture_index(col, row);
    SDL_Texture* texture = window_get_texture(window, index);
    Sprite* sprite = sprite_create(texture);
    window_add_sprite(
        window, 
        sprite, 
        col * TEXTURE_DEFAULT_SIZE_PX, 
        row * TEXTURE_DEFAULT_SIZE_PX
    );

    Tile* tile = board_get_tile_at(ui->board, col, row);
    texture = board_ui_tile_get_texture(ui, col, row);

    Button* button = button_create(
        texture,
        texture,
        texture
    );
    button_set_on_left_click_fn(button, select_tile, ui, tile);
    button_set_on_right_click_fn(button, deselect_tile, ui, 0);
    button_set_size(button, TEXTURE_DEFAULT_SIZE_PX, TEXTURE_DEFAULT_SIZE_PX);
    window_add_button(
        window, 
        button, 
        col * TEXTURE_DEFAULT_SIZE_PX, 
        row * TEXTURE_DEFAULT_SIZE_PX
    );
}
static BoardTextures tile_get_texture_index(int col, int row)
{
    return ((col + row) % 2 == 0) ? TEXTURE_TILE_WHITE : TEXTURE_TILE_BLACK;
}
static void board_ui_add_piece(BoardUI* ui, int col, int row)
{
    verify(ui == NULL, "BoardUI does not exist");

    Board* board = ui->board;
    Window* window = ui->window;
    Piece* piece = board_get_piece_at(board, col, row);

    int index = piece_get_texture_index(piece);
    SDL_Texture* texture = window_get_texture(window, index);
    Sprite* sprite = sprite_create(texture);
    window_add_sprite(
        window, 
        sprite, 
        col * TEXTURE_DEFAULT_SIZE_PX, 
        row * TEXTURE_DEFAULT_SIZE_PX
    );
}
static BoardTextures piece_get_texture_index(const Piece* piece)
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

void board_ui_update(BoardUI* ui)
{
    Window* window = ui->window;
    Board* board = ui->board;

    window_destroy_content(window);

    for (int col = 0; col < board_get_col_num(board); col++)
    {
        for (int row = 0; row < board_get_row_num(board); row++)
        {
            board_ui_add_tile(ui, col, row);

            if (!board_has_piece_at(board, col, row))
            {
                continue;
            }
            board_ui_add_piece(ui, col, row);
        }
    }
}

void select_tile(void* board_ui, void* tile)
{
    BoardUI* ui = (BoardUI*)board_ui;
    Tile* new_tile = (Tile*)tile;
    Board* board = ui->board;
    Tile* old_tile = ui->selected_tile;

    // if (board_ui_has_task(ui))
    // {
    //     ui->selected_tile = NULL;
    //     // TODO:
    // }

    if (new_tile == old_tile)
    {
        ui->selected_tile = NULL;
        board_ui_update(ui);
        return;
    }

    ui->selected_tile = new_tile;
    // tile_ui_set_selected(board->selected_tile);
    board_ui_update(ui);

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
    if (board_can_piece_move_to(board, old_col, old_row, new_col, new_row))
    {
        board_piece_move_to(board, old_col, old_row, new_col, new_row);
        ui->selected_tile = NULL;
        board_ui_update(ui);
        return;
    }

    if (!board_has_piece_at(board, new_col, new_row))
    {
        return;
    }
    if (board_can_piece_capture(board, old_col, old_row, new_col, new_row))
    {
        board_piece_capture(board, old_col, old_row, new_col, new_row);
        board_piece_move_to(board, old_col, old_row, new_col, new_row);
        ui->selected_tile = NULL;
        board_ui_update(ui);
    }
}
void deselect_tile(void* board_ui, void* x)
{
    (void) x;
    BoardUI* ui = (BoardUI*)board_ui;
    ui->selected_tile = NULL;
    board_ui_update(ui);
}

bool board_ui_has_task(BoardUI* ui)
{
    return vector_get_count(ui->tasks) > 0;
}
void board_ui_task_complete_first(BoardUI* ui)
{
    Task* task = vector_get_at(ui->tasks, 0);
    vector_destroy_item(ui->tasks, task);
}
Task board_ui_get_task_first(BoardUI* ui)
{
    return *(Task*)vector_get_at(ui->tasks, 0);
}
void board_ui_add_task(BoardUI* ui, Task task)
{
    Task* task_to_add = SDL_malloc(sizeof(Task));
    verify(task_to_add == NULL, "Task could not be added: malloc");
    *task_to_add = task;
    vector_add(ui->tasks, task_to_add);
}

Window* board_ui_get_window(BoardUI* ui)
{
    return ui->window;
}
