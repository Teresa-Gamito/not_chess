#include "include/game/ui/board_ui.h"

static void board_ui_set_scale(BoardUI* ui);
static void board_ui_add_piece(BoardUI* ui, int col, int row);
static BoardTextures tile_get_texture_index(const Tile* tile);
static BoardTextures piece_get_texture_index(const Piece* piece);

static bool board_ui_is_valid_task_tile(BoardUI* ui, Task task, int col, int row);
static int do_task(BoardUI* ui, Task task, int col, int row);
static void ui_update(BoardUI* ui);

struct BoardUI
{
    Board* board;

    Window* window;

    Tile* selected_tile;
    Vector* tasks;
};

BoardUI* board_ui_create(Board* board, SDL_Renderer* renderer, float x, float y, float width, float height)
{
    verify_renderer(renderer);
    verify_board(board);

    BoardUI* ui = SDL_malloc(sizeof(BoardUI));
    verify(ui == NULL, "BoardUI could not be created: malloc");

    // board
    ui->board = board;

    // window
    SDL_Texture* background_texture = SDL_CreateTextureFromPNG(
        renderer, 
        PATH_TEXTURE_WINDOW_BOARD_BACKGROUND
    );
    ui->window = window_create(
        x, 
        y, 
        width, 
        height, 
        background_texture,
        WINDOW_ZOOMABLE | WINDOW_DRAGGABLE
    );
    window_load_textures(
        renderer, 
        ui->window, 
        board_textures, 
        BOARD_TEXTURE_COUNT
    );
    board_ui_set_scale(ui);

    // selected tile
    ui->selected_tile = NULL;

    // tasks
    ui->tasks = task_list_create();

    // board_ui_update(ui);

    return ui;
}

void board_ui_destroy(BoardUI* ui)
{
    verify_board_ui(ui);

    window_destroy(ui->window);
    task_list_destroy(ui->tasks);
    SDL_free(ui);
}

static void board_ui_set_scale(BoardUI* ui)
{
    Window* window = ui->window;
    float window_width = window_get_width(window);
    float window_height = window_get_height(window);

    Board* board = ui->board;
    float board_width = board_get_col_num(board) * TEXTURE_DEFAULT_SIZE_PX;
    float board_height = board_get_row_num(board) * TEXTURE_DEFAULT_SIZE_PX;

    float new_scale;
    if (board_width / board_height > window_width / window_height)
    {
        new_scale = window_width / board_width;
    }
    else
    {
        new_scale = window_height / board_height;
    }
    window_set_scale(ui->window, new_scale);
}

static SDL_Texture* board_ui_tile_get_texture(BoardUI* ui, int new_col, int new_row)
{
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
    verify_board_ui(ui);
    verify_board_pos(ui->board, col, row);
    Board* board = ui->board;

    Window* window = ui->window;

    Tile* tile = board_get_tile_at(board, col, row);
    int index = tile_get_texture_index(tile);
    SDL_Texture* texture = window_get_texture(window, index);
    Sprite* sprite = sprite_create(texture);
    int x;
    int y;
    if (board_get_active_player(board) == board_get_player_white(board))
    {
        x = col * TEXTURE_DEFAULT_SIZE_PX;
        y = row * TEXTURE_DEFAULT_SIZE_PX;
    }
    else 
    {
        x = (board_get_col_num(board) - 1 - col) * TEXTURE_DEFAULT_SIZE_PX;
        y = (board_get_row_num(board) - 1 - row) * TEXTURE_DEFAULT_SIZE_PX;
    }
    window_add_sprite(
        window, 
        sprite, 
        x,
        y
    );

    texture = board_ui_tile_get_texture(ui, col, row);

    Button* button = button_create(
        texture,
        texture,
        texture
    );

    Function* func_left = function_create(select_tile, ui, tile);
    button_set_on_click_fn(button, MOUSE_LEFT, func_left);

    Function* func_right = function_create(deselect_tile, ui, 0);
    button_set_on_click_fn(button, MOUSE_RIGHT, func_right);

    button_set_size(button, TEXTURE_DEFAULT_SIZE_PX, TEXTURE_DEFAULT_SIZE_PX);

    window_add_button(
        window, 
        button, 
        x,
        y
    );
}

static BoardTextures tile_get_texture_index(const Tile* tile)
{
    return tile_get_color(tile) == WHITE ? TEXTURE_TILE_WHITE : TEXTURE_TILE_BLACK;
}

static void board_ui_add_piece(BoardUI* ui, int col, int row)
{
    Board* board = ui->board;
    Window* window = ui->window;
    Piece* piece = board_get_piece_at(board, col, row);

    int index = piece_get_texture_index(piece);
    SDL_Texture* texture = window_get_texture(window, index);
    Sprite* sprite = sprite_create(texture);
    int x;
    int y;
    if (board_get_active_player(board) == board_get_player_white(board))
    {
        x = col * TEXTURE_DEFAULT_SIZE_PX;
        y = row * TEXTURE_DEFAULT_SIZE_PX;
    }
    else 
    {
        x = (board_get_col_num(board) - 1 - col) * TEXTURE_DEFAULT_SIZE_PX;
        y = (board_get_row_num(board) - 1 - row) * TEXTURE_DEFAULT_SIZE_PX;
    }
    window_add_sprite(
        window, 
        sprite, 
        x,
        y
    );
}

static BoardTextures piece_get_texture_index(const Piece* piece)
{
    Color color = piece_get_color(piece);
    PieceType type = piece_get_type(piece);

    if (color == WHITE)
    {
        switch (type)
        {
            case PAWN:
                return TEXTURE_WHITE_PAWN;

            case ROOK:
                return TEXTURE_WHITE_ROOK;

            case KNIGHT:
                return TEXTURE_WHITE_KNIGHT;

            case BISHOP:
                return TEXTURE_WHITE_BISHOP;

            case QUEEN:
                return TEXTURE_WHITE_QUEEN;

            case KING:
                return TEXTURE_WHITE_KING;

            case LANCE:
                return TEXTURE_WHITE_LANCE;

            case PROMOTED_LANCE:
                return TEXTURE_WHITE_PROMOTED_LANCE;

            default:
                return 0;
        }
    }
    else if (color == BLACK)
    {
        switch (type)
        {
            case PAWN:
                return TEXTURE_BLACK_PAWN;

            case ROOK:
                return TEXTURE_BLACK_ROOK;

            case KNIGHT:
                return TEXTURE_BLACK_KNIGHT;

            case BISHOP:
                return TEXTURE_BLACK_BISHOP;

            case QUEEN:
                return TEXTURE_BLACK_QUEEN;

            case KING:
                return TEXTURE_BLACK_KING;

            case LANCE:
                return TEXTURE_BLACK_LANCE;

            case PROMOTED_LANCE:
                return TEXTURE_BLACK_PROMOTED_LANCE;

            default:
                return 0;
        }
    }
    return 0;
}

static void ui_update(BoardUI* ui)
{
    verify_board_ui(ui);

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

void board_ui_render(SDL_Renderer* renderer, BoardUI* ui)
{
    window_render(renderer, ui->window);
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
    advance_turn(board);
    board_ui_update(ui);
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
    advance_turn(board);
    board_ui_update(ui);

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
    verify_board_ui(ui);
    Tile* new_tile = (Tile*)tile;
    Board* board = ui->board;
    Tile* old_tile = ui->selected_tile;

    int new_col = board_tile_get_col(board, new_tile);
    int new_row = board_tile_get_row(board, new_tile);

    if (try_task(ui, new_col, new_row))
    {
        ui->selected_tile = NULL;
        board_ui_update(ui);
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
        char* log_msg = NULL;
        SDL_asprintf(
            &log_msg, 
            "Capture: %d-%d -> %d-%d",
            old_col,
            old_row,
            new_col,
            new_row
        );
        gamelog_add(ui->log, log_msg);
        SDL_free(log_msg);
        return;
    }
    if (try_piece_move(ui, old_col, old_row, new_col, new_row))
    {
        char* log_msg = NULL;
        SDL_asprintf(
            &log_msg, 
            "Move: %d-%d -> %d-%d",
            old_col,
            old_row,
            new_col,
            new_row
        );
        gamelog_add(ui->log, log_msg);
        SDL_free(log_msg);
        return;
    }


}
void deselect_tile(void* board_ui, void* x)
{
    (void) x;
    BoardUI* ui = (BoardUI*)board_ui;
    verify_board_ui(ui);
    ui->selected_tile = NULL;
    board_ui_update(ui);
}

static bool board_ui_is_valid_task_tile(BoardUI* ui, Task task, int col, int row)
{
     verify_board_ui(ui);

    Board* board = ui->board;

    switch (task) 
    {
        case TASK_ADD_PAWN:
            return task_is_valid_tile__addPiece(board, col, row);

        case TASK_EXPAND_BOARD:
            return task_is_valid_tile__expandBoard(board);

        case TASK_ADD_LANCE:
            return task_is_valid_tile__addPiece(board, col, row);

        case TASK_SACRIFICE:
            return task_is_valid_tile__sacrifice(board, col, row);

        default:
            return false;
    }
    return false;
}

static int do_task(BoardUI* ui, Task task, int col, int row)
{
    verify_board_ui(ui);

    Board* board = ui->board;

    switch (task)
    {
        case TASK_ADD_PAWN:
            return task__addPiece(board, PAWN, col, row);

        case TASK_EXPAND_BOARD:
            return task__expandBoard(board, ui->window);

        case TASK_ADD_LANCE:
            return task__addPiece(board, LANCE, col, row);

        case TASK_SACRIFICE:
            return task__sacrifice(board, col, row);
        default:
            return 0;
    }
    return 0;
}

void board_ui_add_task(BoardUI* ui, Task task)
{
    verify_board_ui(ui);

    add_task(ui->tasks, task);
    ui->selected_tile = NULL;
    board_ui_update(ui);
}



void verify_board_ui(const BoardUI* ui)
{
    verify(ui == NULL, "BoardUI does not exist");
}
