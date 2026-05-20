#include "include/game/ui/board_ui.h"
#include "helper_functions/helper_functions.h"

static void board_ui_set_scale(BoardUI* ui);
static void board_ui_add_piece(BoardUI* ui, Pos pos);
static BoardTextures tile_get_texture_index(const Tile* tile);
static BoardTextures piece_get_texture_index(const Piece* piece);

static bool board_ui_is_valid_task_tile(BoardUI* ui, Task task, Pos pos);
static int do_task(BoardUI* ui, Task task, Pos pos);
static void ui_update(BoardUI* ui);

struct BoardUI
{
    Board* board;

    Window* window;

    Pos selected_pos;
    bool is_selected;

    Vector* tasks;
};

BoardUI* board_ui_create(SDL_Renderer* renderer, Board* board, float x, float y, float width, float height)
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
    ui->selected_pos = (Pos){0, 0};
    ui->is_selected = false;

    // tasks
    ui->tasks = task_list_create();

    ui_update(ui);

    return ui;
}

void board_ui_destroy(BoardUI* ui)
{
    verify_board_ui(ui);

    window_destroy(ui->window);
    task_list_destroy(ui->tasks);
    SDL_free(ui);
}

int board_ui_update(InputState* input, BoardUI* ui)
{
    Board* board = ui->board;
    if (board_game_ended(board))
    {
        Player* player = board_get_active_player(board);
        if (player == board_get_player_white(board))
        {
            SDL_Log("White wins!");
            return 1;
        }
        if (player == board_get_player_black(board))
        {
            SDL_Log("Black wins!");
            return 2;
        }
    }

    window_update(input, ui->window);

    return 0;
}
void board_ui_render(SDL_Renderer* renderer, const BoardUI* ui)
{
    window_render(renderer, ui->window);
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

static SDL_Texture* board_ui_tile_get_texture(BoardUI* ui, Pos pos)
{
    if (has_task(ui->tasks))
    {
        Task task = task_get_first(ui->tasks);
        if (board_ui_is_valid_task_tile(ui, task, pos))
        {
            return window_get_texture(ui->window, TEXTURE_TILE_VALID);
        }
        return NULL;
    }

    if (!ui->is_selected) return NULL;

    Board* board = ui->board;
    Tile* tile = board_get_tile_at(ui->board, pos);

    if (pos_equals(pos, ui->selected_pos))
    {
        return window_get_texture(ui->window, TEXTURE_TILE_SELECTED);
    }

    if (!board_has_piece_at(ui->board, ui->selected_pos)) 
    {
        return NULL;
    }
    Color player_color = player_get_color(board_get_active_player(board));
    Color piece_color = piece_get_color(board_get_piece_at(board, ui->selected_pos));
    if (player_color != piece_color)
    {
        return NULL;
    }

    if (board_can_piece_move_to(board, ui->selected_pos, pos))
    {
        return window_get_texture(ui->window, TEXTURE_TILE_VALID);
    }

    if (!board_has_piece_at(board, pos))
    {
        return NULL;
    }
    if (board_can_piece_capture(board, ui->selected_pos, pos))
    {
        return window_get_texture(ui->window, TEXTURE_TILE_CAPTURE);
    }
    return NULL;
}

static void board_ui_add_tile(BoardUI* ui, Pos pos)
{
    verify_board_ui(ui);
    verify_board_pos(ui->board, pos);

    Board* board = ui->board;
    Window* window = ui->window;

    Tile* tile = board_get_tile_at(board, pos);
    int index = tile_get_texture_index(tile);
    SDL_Texture* texture = window_get_texture(window, index);
    Sprite* sprite = sprite_create(texture);
    int x = col * TEXTURE_DEFAULT_SIZE_PX;
    int y = row * TEXTURE_DEFAULT_SIZE_PX;
    window_add_sprite(window, sprite, x, y);

    texture = board_ui_tile_get_texture(ui, pos);
    Button* button = button_create(
        texture,
        texture,
        texture
    );
    // Function* func_left = function_create(select_tile, ui, tile);
    // button_set_on_click_fn(button, MOUSE_LEFT, func_left);
    // Function* func_right = function_create(deselect_tile, ui, 0);
    // button_set_on_click_fn(button, MOUSE_RIGHT, func_right);
    button_set_size(button, TEXTURE_DEFAULT_SIZE_PX, TEXTURE_DEFAULT_SIZE_PX);
    window_add_button(window, button, x, y);
}

static BoardTextures tile_get_texture_index(const Tile* tile)
{
    return tile_get_color(tile) == WHITE ? TEXTURE_TILE_WHITE : TEXTURE_TILE_BLACK;
}

static void board_ui_add_piece(BoardUI* ui, Pos pos)
{
    Board* board = ui->board;
    Window* window = ui->window;
    Piece* piece = board_get_piece_at(board, pos);

    int index = piece_get_texture_index(piece);
    SDL_Texture* texture = window_get_texture(window, index);
    Sprite* sprite = sprite_create(texture);
    int x = col * TEXTURE_DEFAULT_SIZE_PX;
    int y = row * TEXTURE_DEFAULT_SIZE_PX;
    window_add_sprite(window, sprite, x, y);
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
            Pos pos = {col, row};
            board_ui_add_tile(ui, pos);
            if (board_has_piece_at(board, pos))
            {
                board_ui_add_piece(ui, pos);
            }
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
static int try_piece_move(BoardUI* ui, Pos src, Pos dst)
{
    Board* board = ui->board;

    if (!board_has_piece_at(board, src)) 
    {
        return 0;
    }
    Piece* piece = board_get_piece_at(board, src);
    Player* player = board_get_active_player(board);
    if (!is_piece_player_color(piece, player))
    {
        return 0;
    }
    if (board_has_piece_at(board, dst)) 
    {
        return 0;
    }
    if (!board_can_piece_move_to(board, src, dst))
    {
        return 0;
    }

    board_piece_move_to(board, src, dst);
    ui->is_selected = false;
    advance_turn(board);
    ui_update(ui);
    return 1;
}
static int try_piece_capture(BoardUI* ui, Pos src, Pos dst)
{
    Board* board = ui->board;

    if (!board_has_piece_at(board, src)) 
    {
        return 0;
    }
    Piece* piece = board_get_piece_at(board, src);
    Player* player = board_get_active_player(board);
    if (!is_piece_player_color(piece, player))
    {
        return 0;
    }
    if (!board_has_piece_at(board, dst)) 
    {
        return 0;
    }
    if (!board_can_piece_capture(board, src, dst))
    {
        return 0;
    }

    board_piece_capture(board, board_get_piece_at(board, dst));
    board_piece_move_to(board, src, dst);
    ui->is_selected = false;
    advance_turn(board);
    ui_update(ui);

    return 1;
}
static int try_task(BoardUI* ui, Pos pos)
{
    Board* board = ui->board;

    if (!has_task(ui->tasks))
    {
        return 0;
    }

    Task task = task_get_first(ui->tasks);
    if (!do_task(ui, task, pos))
    {
        return 0;
    }
    task_complete_first(ui->tasks);

    ui_update(ui);
    advance_turn(board);

    return 1;
}
void select_tile(void* board_ui, void* pos)
{
    BoardUI* ui = (BoardUI*)board_ui;
    verify_board_ui(ui);
    verify(pos == NULL, "Invalid position");

    Pos new_pos = *((Pos*)pos);
    Pos old_pos = ui->selected_pos;
    SDL_free(pos);

    if (try_task(ui, new_pos))
    {
        ui->is_selected = false;
        ui_update(ui);
        return;
    }

    // HACK: remove?
    if (pos_equals(new_pos, old_pos))
    {
        ui->is_selected = false;
        ui_update(ui);
        return;
    }

    ui->selected_pos = new_pos;
    ui_update(ui);

    if (try_piece_capture(ui, old_pos, new_pos))
    {
        return;
    }
    if (try_piece_move(ui, old_pos, new_pos))
    {
        return;
    }
}

void deselect_tile(void* board_ui, void* x)
{
    (void) x;
    BoardUI* ui = (BoardUI*)board_ui;
    verify_board_ui(ui);
    ui->is_selected = false;
    ui_update(ui);
}

static bool board_ui_is_valid_task_tile(BoardUI* ui, Task task, Pos pos)
{
     verify_board_ui(ui);

    Board* board = ui->board;

    switch (task) 
    {
        case TASK_ADD_PAWN:
            return task_is_valid_tile__addPiece(board, pos);

        case TASK_EXPAND_BOARD:
            return task_is_valid_tile__expandBoard(board);

        case TASK_ADD_LANCE:
            return task_is_valid_tile__addPiece(board, pos);

        case TASK_SACRIFICE:
            return task_is_valid_tile__sacrifice(board, pos);

        default:
            return false;
    }
    return false;
}

static int do_task(BoardUI* ui, Task task, Pos pos)
{
    verify_board_ui(ui);

    Board* board = ui->board;

    switch (task)
    {
        case TASK_ADD_PAWN:
            return task__addPiece(board, PAWN, pos);

        case TASK_EXPAND_BOARD:
            return task__expandBoard(board, ui->window);

        case TASK_ADD_LANCE:
            return task__addPiece(board, LANCE, pos);

        case TASK_SACRIFICE:
            return task__sacrifice(board, pos);
        default:
            return 0;
    }
    return 0;
}

void board_ui_add_task(BoardUI* ui, Task task)
{
    verify_board_ui(ui);

    add_task(ui->tasks, task);
    ui->is_selected = false;
    ui_update(ui);
}
Board* board_ui_get_board(const BoardUI* ui)
{
    verify_board_ui(ui);

    return ui->board;
}

const char* board_get_selected_properties(const BoardUI* ui)
{
    // TODO: RODRIGO
}



void verify_board_ui(const BoardUI* ui)
{
    verify(ui == NULL, "BoardUI does not exist");
}
