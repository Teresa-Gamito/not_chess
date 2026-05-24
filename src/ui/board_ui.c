#include "include/ui/board_ui.h"
#include "game/player/player.h"

static void board_ui_set_scale(BoardUI* ui);

static BoardResult select_pos(BoardUI* ui, Pos pos);
static void deselect_pos(BoardUI* ui);

static void update(BoardUI* ui);

struct BoardUI
{
    Board* board;

    Window* window;

    Pos selected_pos;
    bool is_selected;

    Player* active_player;

    Queue* upgrades;
};

BoardUI* board_ui_create(SDL_Renderer* renderer, Board* board, Player* active_player, float x, float y, float width, float height)
{
    BoardUI* ui = SDL_malloc(sizeof(BoardUI));
    verify(ui == NULL, "BoardUI could not be created: malloc");

    ui->board = board;

    // HACK: change?
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

    ui->selected_pos = (Pos){0, 0};
    ui->is_selected = false;

    ui->active_player = active_player;

    ui->upgrades = queue_create();

    add_upgrade(ui, UPGRADE_GAMBLING);

    update(ui);

    return ui;
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


void board_ui_destroy(BoardUI* ui)
{
    window_destroy(ui->window);
    queue_destroy(ui->upgrades);
    SDL_free(ui);
}

static Pos get_board_pos_from_screen(const BoardUI* ui, Pos pos)
{
    Window* window = ui->window;
    float x = pos.x;
    float y = pos.y;

    x = (pos.x - window_get_x(window) - window_get_anchor_x(window)) / (window_get_scale(window) * TEXTURE_DEFAULT_SIZE_PX);
    y = (pos.y - window_get_y(window) - window_get_anchor_y(window)) / (window_get_scale(window) * TEXTURE_DEFAULT_SIZE_PX);

    return (Pos){(int)x, (int)y};
}

static BoardResult press_board(InputState* input, BoardUI* ui)
{
    if (mouse_get_released(input, MOUSE_LEFT))
    {
        Pos mouse_pos = (Pos) {mouse_get_x(input), mouse_get_y(input)};
        Pos board_pos = get_board_pos_from_screen(ui, mouse_pos);
        if (board_pos.col >= board_get_col_num(ui->board)) return BOARD_RESULT_CONTINUE;
        if (board_pos.row >= board_get_row_num(ui->board)) return BOARD_RESULT_CONTINUE;
        return select_pos(ui, board_pos);
    }
    if (mouse_get_released(input, MOUSE_RIGHT))
    {
        deselect_pos(ui);
    }
    return BOARD_RESULT_CONTINUE;
}

BoardResult board_ui_update(InputState* input, BoardUI* ui)
{
    window_update(input, ui->window);

    return press_board(input, ui);

    return 0;
}

void board_ui_render(SDL_Renderer* renderer, const BoardUI* ui)
{
    window_render(renderer, ui->window);
}

void board_ui_set_active_player(BoardUI* ui, Player* player)
{
    ui->active_player = player;
}

static SDL_Texture* highlight_get_texture(BoardUI* ui, Pos pos)
{
    Window* window = ui->window;
    Board* board = ui->board;
    Pos selected_pos = ui->selected_pos;

    if (queue_get_size(ui->upgrades) > 0)
    {
        UpgradeType* type = queue_peek(ui->upgrades);
        if (!upgrade_pos_is_valid(*type, board, pos, ui->active_player))
        {
            return NULL;
        }
        return window_get_texture(window, TEXTURE_VALID);
    }

    // no selected tile
    if (!ui->is_selected)
    {
        return NULL;
    }

    // pos is same as selected pos
    if (pos_equals(pos, selected_pos))
    {
        return window_get_texture(window, TEXTURE_SELECTED);
    }

    // no selected piece
    if (!board_has_piece_at(board, selected_pos)) 
    {
        return NULL;
    }

    Color player_color = player_get_color(ui->active_player);
    Color piece_color = piece_get_color(board_get_piece_at(board, selected_pos));

    // opponent piece
    if (player_color != piece_color)
    {
        return NULL;
    }

    // can capture
    if (board_has_piece_at(board, pos))
    {
        if (board_can_piece_capture(board, selected_pos, pos))
        {
            return window_get_texture(window, TEXTURE_CAPTURE);
        }
    }

    // can move to
    if (board_can_piece_move_to(board, selected_pos, pos))
    {
        return window_get_texture(window, TEXTURE_VALID);
    }

    return NULL;
}

static void add_highlight(BoardUI* ui, Pos pos)
{
    float x = pos.col * TEXTURE_DEFAULT_SIZE_PX;
    float y = pos.row * TEXTURE_DEFAULT_SIZE_PX;

    SDL_Texture* texture = highlight_get_texture(ui, pos);
    Sprite* sprite = sprite_create(texture);

    window_add_sprite(ui->window, sprite, x, y);
}

static SDL_Texture* tile_get_texture(BoardUI* ui, Pos pos)
{
    Window* window = ui->window;
    Tile* tile = board_get_tile_at(ui->board, pos);

    TileType type = tile_get_type(tile);
    Color color = tile_get_color(tile);

    int index;
    switch (type)
    {
        case TILE_WALL:
            index = TEXTURE_TILE_WALL;
            break;
        case TILE_POISON:
            index = TEXTURE_TILE_POISON;
            break;
        default:
            index = color == WHITE ? TEXTURE_TILE_WHITE : TEXTURE_TILE_BLACK;
            break;
    }
    return window_get_texture(window, index);
}

static void add_tile(BoardUI* ui, Pos pos)
{
    float x = pos.col * TEXTURE_DEFAULT_SIZE_PX;
    float y = pos.row * TEXTURE_DEFAULT_SIZE_PX;

    SDL_Texture* texture = tile_get_texture(ui, pos);
    Sprite* sprite = sprite_create(texture);

    window_add_sprite(ui->window, sprite, x, y);
}

static SDL_Texture* piece_get_texture(BoardUI* ui, Pos pos)
{
    Piece* piece = board_get_piece_at(ui->board, pos);

    Color color = piece_get_color(piece);
    PieceType type = piece_get_type(piece);

    int index;
    switch (type)
    {
        case PAWN:
            index = color == WHITE ? TEXTURE_WHITE_PAWN : TEXTURE_BLACK_PAWN;
            break;
        case ROOK:
            index = color == WHITE ? TEXTURE_WHITE_ROOK : TEXTURE_BLACK_ROOK;
            break;
        case KNIGHT:
            index = color == WHITE ? TEXTURE_WHITE_KNIGHT : TEXTURE_BLACK_KNIGHT;
            break;
        case BISHOP:
            index = color == WHITE ? TEXTURE_WHITE_BISHOP : TEXTURE_BLACK_BISHOP;
            break;
        case QUEEN:
            index = color == WHITE ? TEXTURE_WHITE_QUEEN : TEXTURE_BLACK_QUEEN;
            break;
        case KING:
            index = color == WHITE ? TEXTURE_WHITE_KING : TEXTURE_BLACK_KING;
            break;
        case LANCE:
            index = color == WHITE ? TEXTURE_WHITE_LANCE : TEXTURE_BLACK_LANCE;
            break;
        case P_LANCE:
            index = color == WHITE ? TEXTURE_WHITE_P_LANCE : TEXTURE_BLACK_P_LANCE;
            break;
        default:
            return 0;
    }
    return window_get_texture(ui->window, index);
}

static void add_piece(BoardUI* ui, Pos pos)
{
    float x = pos.col * TEXTURE_DEFAULT_SIZE_PX;
    float y = pos.row * TEXTURE_DEFAULT_SIZE_PX;

    SDL_Texture* texture = piece_get_texture(ui, pos);
    Sprite* sprite = sprite_create(texture);

    window_add_sprite(ui->window, sprite, x, y);
}

static void update(BoardUI* ui)
{
    window_destroy_content(ui->window);

    for (int col = 0; col < board_get_col_num(ui->board); col++)
    {
        for (int row = 0; row < board_get_row_num(ui->board); row++)
        {
            Pos pos = {col, row};
            add_tile(ui, pos);
            add_highlight(ui, pos);
            if (!board_has_piece_at(ui->board, pos))
            {
                continue;
            }
            add_piece(ui, pos);
        }
    }
    board_ui_set_scale(ui);
}

static bool try_capture(BoardUI* ui, Pos src, Pos dst)
{
    Board* board = ui->board;

    if (!board_has_piece_at(board, src)) return false;
    if (!board_has_piece_at(board, dst)) return false;
    if (!board_can_piece_capture(board, src, dst)) return false;

    Piece* piece = board_get_piece_at(board, src);
    Color player_color = player_get_color(ui->active_player);
    Color piece_color = piece_get_color(piece);
    if (player_color != piece_color) return false;

    int points = board_piece_move_to(board, src, dst);
    player_add_points(ui->active_player, points);
    ui->is_selected = false;
    update(ui);
    return true;
}

static bool try_move(BoardUI* ui, Pos src, Pos dst)
{
    Board* board = ui->board;

    if (!board_has_piece_at(board, src)) return false;
    if (!board_can_piece_move_to(board, src, dst)) return false;

    Color player_color = player_get_color(ui->active_player);
    Color piece_color = piece_get_color(board_get_piece_at(board, src));
    if (player_color != piece_color) return false;

    board_piece_move_to(board, src, dst);
    ui->is_selected = false;
    update(ui);
    return true;
}

static bool try_upgrade(BoardUI* ui, Pos pos)
{
    if (queue_get_size(ui->upgrades) <= 0)
    {
        return false;
    }

    UpgradeType* type = (UpgradeType*)queue_peek(ui->upgrades);
    if (!upgrade_pos_is_valid(*type, ui->board, pos, ui->active_player))
    {
        return false;
    }

    upgrade(*type, ui->board, pos, ui->active_player);
    SDL_free(queue_pop(ui->upgrades));
    ui->is_selected = false;
    update(ui);
    return true;
}

static BoardResult select_pos(BoardUI* ui, Pos pos)
{
    Pos old_pos = ui->selected_pos;
    Pos new_pos = pos;

    if (try_upgrade(ui, new_pos)) return BOARD_RESULT_ADVANCE_TURN;
    if (try_capture(ui, old_pos, new_pos))
    {
        if (is_check_mate(ui->board, player_get_color(ui->active_player)))
        {
            Color color = player_get_color(ui->active_player);
            return color == WHITE ? BOARD_RESULT_WIN_WHITE : BOARD_RESULT_WIN_BLACK;
        }
        return BOARD_RESULT_ADVANCE_TURN;
    }
    if (try_move(ui, old_pos, new_pos)) return BOARD_RESULT_ADVANCE_TURN;

    ui->selected_pos = new_pos;
    ui->is_selected = true;
    update(ui);
    return BOARD_RESULT_CONTINUE;
}

static void deselect_pos(BoardUI* ui)
{
    verify_board_ui(ui);
    ui->is_selected = false;
    update(ui);
}

void add_upgrade(BoardUI* ui, UpgradeType upgrade)
{
    UpgradeType* type = SDL_malloc(sizeof(UpgradeType));
    *type = upgrade;
    queue_push(ui->upgrades, type);
    if (!upgrade_needs_select(upgrade))
    {
        try_upgrade(ui, ui->selected_pos);
    }
}

Board* board_ui_get_board(const BoardUI* ui)
{
    verify_board_ui(ui);

    return ui->board;
}

const char* board_get_selected_properties(const BoardUI* ui)
{
    // TODO: RODRIGO
    return NULL;
}

void verify_board_ui(const BoardUI* ui)
{
    verify(ui == NULL, "BoardUI does not exist");
}
