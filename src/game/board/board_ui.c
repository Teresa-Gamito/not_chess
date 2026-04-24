#include "include/game/board/board_ui.h"
#include "game/board/board.h"

static void board_ui_add_tile(BoardUI* ui, int col, int row);
static void board_ui_add_piece(BoardUI* ui, int col, int row);
static BoardTextures tile_get_texture_index(int col, int row);
static BoardTextures piece_get_texture_index(const Piece* piece);

static bool board_ui_is_valid_task_tile(BoardUI* ui, Task task, int col, int row);
static int do_task(BoardUI* ui, Task task, int col, int row);

struct BoardUI
{
    Board* board;

    Window* window;

    Tile* selected_tile;
    Vector* tasks;
};

BoardUI* board_ui_create(
    SDL_Renderer* renderer,
    Board* board,
    float x,
    float y,
    float width,
    float height
)
{
    verify(renderer == NULL, "SDL_Renderer does not exist");
    verify(board == NULL, "Board does not exist");

    BoardUI* ui = SDL_malloc(sizeof(BoardUI));
    verify(ui == NULL, "BoardUI could not be created: malloc");

    ui->board = board;

    SDL_Texture* background_texture = SDL_CreateTextureFromPNG(
        renderer, 
        PATH_TEXTURE_WINDOW_BOARD_BACKGROUND
    );
    ui->window = window_create(x, y, width, height, background_texture);
    window_load_textures(
        renderer, 
        ui->window, 
        board_textures, 
        BOARD_TEXTURE_COUNT
    );

    ui->selected_tile = NULL;

    ui->tasks = task_list_create();

    int col_num = board_get_col_num(board);
    int row_num = board_get_row_num(board);
    if ((double) col_num / row_num > width / height)
        window_set_scale(ui->window, width / (col_num * TEXTURE_DEFAULT_SIZE_PX));
    else
        window_set_scale(ui->window, height / (row_num * TEXTURE_DEFAULT_SIZE_PX));

    board_ui_update(ui);

    return ui;
}
void board_ui_destroy(BoardUI* ui)
{
    verify(ui == NULL, "BoardUI does not exist");

    window_destroy(ui->window);
    task_list_destroy(ui->tasks);
    SDL_free(ui);
}
static SDL_Texture* board_ui_tile_get_texture(BoardUI* ui, int new_col, int new_row)
{
    verify(ui == NULL, "BoardUI does not exist");
    verify(new_col < 0 || new_col > board_get_col_num(ui->board), "Invalid position");
    verify(new_row < 0 || new_row > board_get_row_num(ui->board), "Invalid position");

    if (has_task(ui->tasks))
    {
        Task task = task_get_first(ui->tasks);
        if (board_ui_is_valid_task_tile(ui, task, new_col, new_row))
        {
            return window_get_texture(ui->window, TEXTURE_TILE_VALID);
        }
        return NULL;
    }

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
    Color player_color = player_get_color(board_get_active_player(board));
    Color piece_color = piece_get_color(board_get_piece_at(board, old_col, old_row));
    if (player_color != piece_color)
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

    Color color = piece_get_color(piece);
    PieceType type = piece_get_type(piece);

    if (color == WHITE)
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
    else if (color == BLACK)
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
    verify(ui == NULL, "BoardUI does not exist");

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

static bool is_piece_player_color(Piece* piece, Player* player)
{
    Color player_color = player_get_color(player);
    Color piece_color = piece_get_color(piece);
    if (player_color != piece_color)
    {
        return false;
    }
    return true;
}
static int try_piece_move(BoardUI* ui, int src_col, int src_row, int dst_col, int dst_row)
{
    Board* board = ui->board;

    if (!board_has_piece_at(board, src_col, src_row)) 
    {
        return 0;
    }
    Piece* piece = board_get_piece_at(board, src_col, src_row);
    Player* player = board_get_active_player(board);
    if (!is_piece_player_color(piece, player))
    {
        return 0;
    }
    if (board_has_piece_at(board, dst_col, dst_row)) 
    {
        return 0;
    }
    if (!board_can_piece_move_to(board, src_col, src_row, dst_col, dst_row))
    {
        return 0;
    }

    board_piece_move_to(board, src_col, src_row, dst_col, dst_row);
    ui->selected_tile = NULL;
    board_ui_update(ui);
    advance_turn(board);
    return 1;
}
static int try_piece_capture(BoardUI* ui, int src_col, int src_row, int dst_col, int dst_row)
{
    Board* board = ui->board;

    if (!board_has_piece_at(board, src_col, src_row)) 
    {
        return 0;
    }
    Piece* piece = board_get_piece_at(board, src_col, src_row);
    Player* player = board_get_active_player(board);
    if (!is_piece_player_color(piece, player))
    {
        return 0;
    }
    if (!board_has_piece_at(board, dst_col, dst_row)) 
    {
        return 0;
    }
    if (!board_can_piece_capture(board, src_col, src_row, dst_col, dst_row))
    {
        return 0;
    }

    board_piece_capture(board, board_get_piece_at(board, dst_col, dst_row));
    board_piece_move_to(board, src_col, src_row, dst_col, dst_row);
    ui->selected_tile = NULL;
    board_ui_update(ui);
    advance_turn(board);

    return 1;
}
static int try_task(BoardUI* ui, int col, int row)
{
    Board* board = ui->board;

    if (!has_task(ui->tasks))
    {
        return 0;
    }

    Task task = task_get_first(ui->tasks);
    if (!do_task(ui, task, col, row))
    {
        return 0;
    }

    task_complete_first(ui->tasks);
    board_ui_update(ui);
    advance_turn(board);

    return 1;
}
void select_tile(void* board_ui, void* tile)
{
    BoardUI* ui = (BoardUI*)board_ui;
    verify(ui == NULL, "BoardUI does not exist");
    Tile* new_tile = (Tile*)tile;
    Board* board = ui->board;
    Tile* old_tile = ui->selected_tile;

    int new_col = board_tile_get_col(board, new_tile);
    int new_row = board_tile_get_row(board, new_tile);

    if (try_task(ui, new_col, new_row))
    {
        return;
    }

    if (new_tile == old_tile)
    {
        ui->selected_tile = NULL;
        board_ui_update(ui);
        return;
    }

    ui->selected_tile = new_tile;
    board_ui_update(ui);

    if (new_tile == NULL) return;
    if (old_tile == NULL) return;

    int old_col = board_tile_get_col(board, old_tile);
    int old_row = board_tile_get_row(board, old_tile);

    if (try_piece_capture(ui, old_col, old_row, new_col, new_row))
    {
        return;
    }
    if (try_piece_move(ui, old_col, old_row, new_col, new_row))
    {
        return;
    }


}
void deselect_tile(void* board_ui, void* x)
{
    (void) x;
    BoardUI* ui = (BoardUI*)board_ui;
    verify(ui == NULL, "BoardUI does not exist");
    ui->selected_tile = NULL;
    board_ui_update(ui);
}

static bool board_ui_is_valid_task_tile(BoardUI* ui, Task task, int col, int row)
{
    verify(ui == NULL, "BoardUI does not exist");

    Board* board = ui->board;

    switch (task) 
    {
        case TASK_ADD_PAWN:
            return task_is_valid_tile__addPawn(ui->board, col, row);
            break;

        default:
            return false;
            break;
    }
    return false;
}

static int do_task(BoardUI* ui, Task task, int col, int row)
{
    verify(ui == NULL, "BoardUI does not exist");

    switch (task)
    {
        case TASK_ADD_PAWN:
            return task__addPawn(ui->board, col, row);
            break;

        default:
            return 0;
            break;
    }
    return 0;
}

void board_ui_add_task(BoardUI* ui, Task task)
{
    verify(ui == NULL, "BoardUI does not exist");

    add_task(ui->tasks, task);
    ui->selected_tile = NULL;
    board_ui_update(ui);
}

Window* board_ui_get_window(const BoardUI* ui)
{
    verify(ui == NULL, "BoardUI does not exist");

    return ui->window;
}
Board* board_ui_get_board(const BoardUI* ui)
{
    verify(ui == NULL, "BoardUI does not exist");

    return ui->board;
}
