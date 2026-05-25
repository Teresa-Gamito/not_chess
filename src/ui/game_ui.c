#include "ui/game_ui.h"

typedef enum GameScreen
{
    SCREEN_BOARD,
    SCREEN_TREE,

    SCREEN_COUNT,
} GameScreen;

typedef struct UIState
{
    bool is_paused;
    bool show_properties;
    bool show_rules;
    bool show_log;
} UIState;

struct GameUI 
{
    Game* game;

    BoardUI* board_ui;
    TreeUI* tree_ui;

    Window* ui_buttons;
    Window* ui_properties;
    Window* ui_player_info;
    Window* ui_rules;
    Window* ui_mini_tree;
    Window* ui_pause;
    Window* ui_log;

    TTF_Font* ui_font_small;
    TTF_Font* ui_font_medium;
    TTF_Font* ui_font_large;

    SDL_Color* ui_text_color;

    GameScreen screen;
    UIState* state;
};

static void toggle_screen(GameUI* ui);
static void toggle_pause(void* game_ui, void* null);

static Window* create_ui_buttons(SDL_Renderer* renderer, GameUI* ui);
static Window* create_ui_properties(SDL_Renderer* renderer);
static void update_ui_properties(SDL_Renderer* renderer, Window* window, const GameUI* ui);
static Window* create_ui_player_info(SDL_Renderer* renderer);
static void update_ui_player_info(SDL_Renderer* renderer, Window* window, const GameUI* ui);
static Window* create_ui_rules(SDL_Renderer* renderer);
static void update_ui_rules(SDL_Renderer* renderer, Window* window, const GameUI* ui);

GameUI* game_ui_create(Game* game, SDL_Renderer* renderer)
{
    GameUI* ui = SDL_malloc(sizeof(GameUI));
    verify(ui == NULL, "GameUI could not be created: malloc");

    ui->game = game;

    ui->board_ui = board_ui_create(
        renderer,
        game,
        0,
        0,
        g_app_window_width,
        g_app_window_height
    );

    ui->tree_ui = tree_ui_create(
        renderer, 
        game,
        0, 
        0, 
        g_app_window_width, 
        g_app_window_height
    );

    ui->screen = SCREEN_BOARD;
    ui->state = SDL_malloc(sizeof(UIState));
    *ui->state = (UIState){false, false, false, false};

    ui->ui_buttons = create_ui_buttons(renderer, ui);
    ui->ui_player_info = create_ui_player_info(renderer);
    ui->ui_properties = create_ui_properties(renderer);
    ui->ui_rules = create_ui_rules(renderer);

    ui->ui_font_small = TTF_OpenFont(FONT_PATH, FONT_SIZE * (g_app_scale - 0.4));
    ui->ui_font_medium = TTF_OpenFont(FONT_PATH, FONT_SIZE * (g_app_scale));
    ui->ui_font_large = TTF_OpenFont(FONT_PATH, FONT_SIZE * (g_app_scale + 0.1));

    ui->ui_text_color = SDL_malloc(sizeof(SDL_Color));
    *ui->ui_text_color = color_white();

    return ui;
}

void game_ui_destroy(GameUI* ui)
{
    verify_game_ui(ui);

    board_ui_destroy(ui->board_ui);
    tree_ui_destroy(ui->tree_ui);

    window_destroy(ui->ui_buttons);
    window_destroy(ui->ui_player_info);
    window_destroy(ui->ui_properties);
    window_destroy(ui->ui_rules);

    TTF_CloseFont(ui->ui_font_small);
    TTF_CloseFont(ui->ui_font_medium);
    TTF_CloseFont(ui->ui_font_large);

    SDL_free(ui->ui_text_color);

    SDL_free(ui);
}

void game_ui_render(SDL_Renderer* renderer, const GameUI* ui)
{
    verify_renderer(renderer);
    verify_game_ui(ui);

    if (ui->screen == SCREEN_BOARD)
    {
        board_ui_render(renderer, ui->board_ui);

        window_render(renderer, ui->ui_buttons);

        update_ui_player_info(renderer, ui->ui_player_info, ui);
        window_render(renderer, ui->ui_player_info);

        update_ui_properties(renderer, ui->ui_properties, ui);
        if (ui->state->show_properties)
        {
            window_render(renderer, ui->ui_properties);
        }

        if (ui->state->show_rules)
        {
            update_ui_rules(renderer, ui->ui_rules, ui);
            window_render(renderer, ui->ui_rules);
        }
    }
    if (ui->screen == SCREEN_TREE)
    {
        tree_ui_render(renderer, ui->tree_ui);
    }
}

static void update_keys(InputState* input, GameUI* ui)
{
    if (keyboard_get_released(input, SDL_SCANCODE_ESCAPE))
    {
        toggle_pause(ui, NULL);
    }
    if (keyboard_get_released(input, SDL_SCANCODE_TAB) && !ui->state->is_paused)
    {
        toggle_screen(ui);
    }
    if (mouse_get_released(input, MOUSE_RIGHT))
    {
        if (ui->state->is_paused)
        {
            ui->state->is_paused = false;
            return;
        }
        if (ui->state->show_rules)
        {
            ui->state->show_rules = false;
            return;
        }
        if (ui->state->show_log)
        {
            ui->state->show_log = false;
            return;
        }
        if (board_ui_has_selected_pos(ui->board_ui))
        {
            ui->state->show_properties = false;
            deselect_pos(ui->board_ui);
            return;
        }
    }
}

GameResult game_ui_update(InputState* input, GameUI* ui)
{
    verify_game_ui(ui);
    verify_input(input);

    update_keys(input, ui);

    GameResult result = GAME_RESULT_CONTINUE;
    if (ui->screen == SCREEN_BOARD)
    {
        board_ui_update(input, ui->board_ui);
        window_update(input, ui->ui_buttons);
        if (ui->state->show_properties)
        {
            window_update(input, ui->ui_properties);
        }
    }
    if (ui->screen == SCREEN_TREE)
    {
        tree_ui_update(input, ui->tree_ui);
    }

    return result;
}

static void toggle_screen(GameUI* ui)
{
    ui->screen++;
    if (ui->screen >= SCREEN_COUNT) ui->screen = 0;
    board_ui_update_objects(ui->board_ui);
}


static void toggle_pause(void* game_ui, void* null)
{
    GameUI* ui = (GameUI*)game_ui;
    ui->state->show_rules = false;
    ui->state->show_log = false;
    ui->state->is_paused = !ui->state->is_paused;
}
static void toggle_rules(void* game_ui, void* null)
{
    GameUI* ui = (GameUI*)game_ui;
    ui->state->show_rules = !ui->state->show_rules;
    ui->state->show_log = false;
    ui->state->is_paused = false;
}
static void toggle_log(void* game_ui, void* null)
{
    GameUI* ui = (GameUI*)game_ui;
    ui->state->show_rules = false;
    ui->state->show_log = !ui->state->show_log;
    ui->state->is_paused = false;
}
static void toggle_help(void* game_ui, void* null) {} // not implemented
static Window* create_ui_buttons(SDL_Renderer* renderer, GameUI* ui)
{
    int button_count = 4;

    float window_width = button_count * UI_BUTTON_SIZE + (button_count - 1) * UI_BUFFER;
    Window* window = window_create(
        screen_width - window_width - UI_BUFFER,
        UI_BUFFER,
        window_width,
        UI_BUTTON_SIZE,
        NULL,
        0
    );
    window_load_textures(renderer, window, ui_buttons_textures, UI_BUTTONS_TEXTURE_COUNT);

    Button* button;
    SDL_Texture* texture;
    Function* func;
    for (int i = 0; i < button_count; i++)
    {
        UIButtonsTextures index = 0;
        switch (i)
        {
            case 0:
                index = TEXTURE_BUTTON_RULES;
                func = function_create(toggle_rules, ui, NULL);
                break;
            case 1:
                index = TEXTURE_BUTTON_LOG;
                func = function_create(toggle_log, ui, NULL);
                break;
            case 2:
                index = TEXTURE_BUTTON_HELP;
                func = function_create(toggle_help, ui, NULL);
                break;
            case 3:
                index = TEXTURE_BUTTON_PAUSE;
                func = function_create(toggle_pause, ui, NULL);
                break;
        }
        texture = window_get_texture(window, index);
        button = button_create(
            texture,
            texture,
            NULL
        );
        button_set_on_click_fn(button, MOUSE_LEFT, func);
        button_set_size(button, UI_BUTTON_SIZE, UI_BUTTON_SIZE);
        window_add_button(window, button, i * (UI_BUTTON_SIZE + UI_BUFFER), 0);
    }

    return window;
}

static Window* create_ui_properties(SDL_Renderer* renderer)
{
    Window* window = window_create(
        UI_BUFFER,
        screen_height - UI_PROPERTIES_HEIGHT - UI_BUFFER,
        UI_PROPERTIES_WIDTH,
        UI_PROPERTIES_HEIGHT,
        SDL_CreateTextureFromPNG(renderer, PATH_TEXTURE_WINDOW_PROPERTIES),
        WINDOW_SCROLLABLE
    );
    return window;
}
static void update_ui_properties(SDL_Renderer* renderer, Window* window, const GameUI* ui)
{
    if (!board_ui_has_selected_pos(ui->board_ui))
    {
        ui->state->show_properties = false;
        return;
    }

    ui->state->show_properties = true;

    Pos pos = board_ui_get_selected_pos(ui->board_ui);
    Board* board = game_get_board(ui->game);
    SDL_Texture* texture;
    const char* description;
    if (board_has_piece_at(board, pos))
    {
        Piece* piece = board_get_piece_at(board, pos);
        description = get_piece_properties(piece);
        texture = piece_get_texture(ui->board_ui, pos);
    }
    else
    {
        Tile* tile = board_get_tile_at(board, pos);
        description = get_tile_properties(tile);
        texture = tile_get_texture(ui->board_ui, pos);
    }

    window_destroy_content(window);

    Textbox* textbox = textbox_create(
        renderer,
        ui->ui_font_small,
        ui->ui_text_color,
        description,
        window_get_width(window) - 2 * UI_BUFFER,
        TEXT_LEFT_ALIGNED
    );
    window_add_textbox(window, textbox, UI_BUFFER, UI_BUFFER);

    Sprite* sprite = sprite_create(texture);
    sprite_set_size(sprite, TEXTURE_DEFAULT_SIZE_PX * 8, TEXTURE_DEFAULT_SIZE_PX * 8);
    window_add_sprite(window, sprite, window_get_width(window) - UI_BUFFER - sprite_get_width(sprite), UI_BUFFER);
}

static Window* create_ui_player_info(SDL_Renderer* renderer)
{
    Window* window = window_create(
        UI_BUFFER,
        UI_BUFFER,
        UI_PLAYER_INFO_WIDTH,
        UI_PLAYER_INFO_HEIGHT,
        SDL_CreateTextureFromPNG(renderer, PATH_TEXTURE_WINDOW_PLAYER_INFO),
        0
    );
    return window;
}
static void update_ui_player_info(SDL_Renderer* renderer, Window* window, const GameUI* ui)
{
    window_destroy_content(window);

    Player* player_active = game_get_active_player(ui->game);
    const char* text_player = get_player_properties(player_active);
    Textbox* textbox_player = textbox_create(
        renderer,
        ui->ui_font_large,
        ui->ui_text_color,
        text_player,
        window_get_width(window) - UI_BUFFER * 2,
        TEXT_LEFT_ALIGNED
    );
    window_add_textbox(
        window, 
        textbox_player, 
        UI_BUFFER, 
        UI_BUFFER
    );

    Player* player_opponent = game_get_opponent(ui->game);
    const char* text_opponent = get_player_properties(player_opponent);
    Textbox* textbox_opponent = textbox_create(
        renderer,
        ui->ui_font_small,
        ui->ui_text_color,
        text_opponent,
        window_get_width(window) - UI_BUFFER * 2,
        TEXT_LEFT_ALIGNED
    );
    window_add_textbox(
        window, 
        textbox_opponent, 
        UI_BUFFER,
        textbox_get_height(textbox_player) + UI_BUFFER * 2
    );
}

static Window* create_ui_rules(SDL_Renderer* renderer)
{
    Window* window = window_create(
        screen_width - UI_RULES_WIDTH - UI_BUFFER,
        UI_BUFFER * 2 + UI_BUTTON_SIZE,
        UI_RULES_WIDTH,
        UI_RULES_HEIGHT,
        SDL_CreateTextureFromPNG(renderer, PATH_TEXTURE_WINDOW_RULES),
        0
    );
    return window;
}
static void update_ui_rules(SDL_Renderer* renderer, Window* window, const GameUI* ui)
{
    window_destroy_content(window);

    const char* text = rulelist_get_rules(game_get_rules(ui->game));
    Textbox* textbox = textbox_create(
        renderer,
        ui->ui_font_small,
        ui->ui_text_color,
        text,
        window_get_width(window) - UI_BUFFER * 2,
        TEXT_LEFT_ALIGNED
    );
    window_add_textbox(window, textbox, UI_BUFFER, UI_BUFFER);
}

Window* create_ui_mini_tree();
void update_ui_mini_tree(Window* window);

Window* create_ui_pause();







void verify_game_ui(const GameUI* ui)
{
    verify(ui == NULL, "GameUI does not exist");
}
