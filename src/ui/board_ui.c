#include "include/ui/board_ui.h"

struct BoardUI
{
    Game* game;

    Window* window;

    Sound* sound_move;
    Sound* sound_capture;

    Pos selected_pos;
    bool is_selected;
};

BoardUI* board_ui_create(SDL_Renderer* renderer, Game* game, float x, float y, float width, float height)
{
    BoardUI* ui = SDL_malloc(sizeof(BoardUI));
    verify(ui == NULL, "BoardUI could not be created: malloc");

    ui->game = game;

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

    board_ui_update_objects(ui);

    ui->sound_move = sound_load(PATH_SOUND_MOVE);
    ui->sound_capture = sound_load(PATH_SOUND_CAPTURE);

    return ui;
}

void board_ui_set_scale(BoardUI* ui)
{
    Window* window = ui->window;
    float window_width = window_get_width(window);
    float window_height = window_get_height(window);

    Board* board = game_get_board(ui->game);
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
    sound_unload(ui->sound_move);
    sound_unload(ui->sound_capture);
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

static void press_board(InputState* input, BoardUI* ui)
{
    Board* board = game_get_board(ui->game);
    Window* window = ui->window;
    if (mouse_get_released(input, MOUSE_LEFT))
    {
        Pos mouse_pos = (Pos) {mouse_get_x(input), mouse_get_y(input)};
        Pos board_pos = get_board_pos_from_screen(ui, mouse_pos);
        if (mouse_pos.x < window_get_x(window) + window_get_anchor_x(window)) return;
        if (mouse_pos.y < window_get_y(window) + window_get_anchor_y(window)) return;
        if (mouse_pos.x > window_get_x(window) + window_get_anchor_x(window) + board_get_col_num(board) * TEXTURE_DEFAULT_SIZE_PX * window_get_scale(window)) return;
        if (mouse_pos.y > window_get_y(window) + window_get_anchor_y(window) + board_get_col_num(board) * TEXTURE_DEFAULT_SIZE_PX * window_get_scale(window)) return;
        select_pos(ui, board_pos);
    }
}

void board_ui_update(InputState* input, BoardUI* ui)
{
    window_update(input, ui->window);

    press_board(input, ui);
}

void board_ui_render(SDL_Renderer* renderer, const BoardUI* ui)
{
    window_render(renderer, ui->window);
}

static SDL_Texture* highlight_get_texture(BoardUI* ui, Pos pos)
{
    Player* player = game_get_active_player(ui->game);
    Window* window = ui->window;
    Board* board = game_get_board(ui->game);
    Pos selected_pos = ui->selected_pos;

    Queue* upgrades = game_get_upgrade_queue(ui->game);
    if (queue_get_size(upgrades) > 0)
    {
        UpgradeType* type = queue_peek(upgrades);
        if (!upgrade_pos_is_valid(ui->game, *type, pos))
        {
            return NULL;
        }
        return window_get_texture(window, TEXTURE_VALID);
    }
    if (!ui->is_selected)
    {
        return NULL;
    }
    if (pos_equals(pos, selected_pos))
    {
        return window_get_texture(window, TEXTURE_SELECTED);
    }
    if (!board_has_piece_at(board, selected_pos)) 
    {
        return NULL;
    }

    Color player_color = player_get_color(player);
    Color piece_color = piece_get_color(board_get_piece_at(board, selected_pos));
    if (player_color != piece_color)
    {
        return NULL;
    }
    if (board_has_piece_at(board, pos))
    {
        if (board_can_piece_capture(board, selected_pos, pos))
        {
            return window_get_texture(window, TEXTURE_CAPTURE);
        }
    }
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

SDL_Texture* tile_get_texture(BoardUI* ui, Pos pos)
{
    Window* window = ui->window;
    Board* board = game_get_board(ui->game);
    Tile* tile = board_get_tile_at(board, pos);

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

SDL_Texture* piece_get_texture(BoardUI* ui, Pos pos)
{
    Board* board = game_get_board(ui->game);
    Piece* piece = board_get_piece_at(board, pos);

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

void board_ui_update_objects(BoardUI* ui)
{
    Board* board = game_get_board(ui->game);
    window_destroy_content(ui->window);

    for (int col = 0; col < board_get_col_num(board); col++)
    {
        for (int row = 0; row < board_get_row_num(board); row++)
        {
            Pos pos = {col, row};
            add_tile(ui, pos);
            add_highlight(ui, pos);
            if (!board_has_piece_at(board, pos))
            {
                continue;
            }
            add_piece(ui, pos);
        }
    }
}

void select_pos(BoardUI* ui, Pos pos)
{
    Board* board = game_get_board(ui->game);
    Player* player = game_get_active_player(ui->game);

    Pos old_pos = ui->selected_pos;
    Pos new_pos = pos;

    if (game_try_upgrade(ui->game, new_pos)) 
    {
        game_advance_turn(ui->game);
    }
    else if (game_try_capture(ui->game, old_pos, new_pos))
    {
        game_advance_turn(ui->game);
        sound_play(ui->sound_capture);
    }
    else if (game_try_move(ui->game, old_pos, new_pos))
    {
        sound_play(ui->sound_move);
        game_advance_turn(ui->game);
    }

    ui->selected_pos = new_pos;
    ui->is_selected = true;
    board_ui_update_objects(ui);
    return;
}

void deselect_pos(BoardUI* ui)
{
    verify_board_ui(ui);
    ui->is_selected = false;
    board_ui_update_objects(ui);
}

bool board_ui_has_selected_pos(const BoardUI* ui)
{
    return ui->is_selected;
}

Pos board_ui_get_selected_pos(const BoardUI* ui)
{
    return ui->selected_pos;
}

Window* board_ui_get_window(const BoardUI* ui)
{
    return ui->window;
}

void verify_board_ui(const BoardUI* ui)
{
    verify(ui == NULL, "BoardUI does not exist");
}
