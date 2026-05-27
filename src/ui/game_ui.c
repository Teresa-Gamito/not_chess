#include "ui/game_ui.h"
#include "helper_functions/function.h"
#include "helper_functions/global_variables.h"
#include "ui/ui_elements/sound.h"

typedef enum MenuPauseScreen
{
    MENU_PAUSE_MAIN,
    MENU_PAUSE_OPTIONS,
    MENU_PAUSE_EXIT
} MenuPauseScreen;

typedef enum GameScreen
{
    SCREEN_BOARD,
    SCREEN_TREE
} GameScreen;

typedef struct UIState
{
    bool is_paused;
    bool show_properties;
    bool show_rules;
    bool show_log;
    bool show_help;
    bool show_upgrade_description;
    bool exit_game;
} UIState;

struct GameUI 
{
    Game* game;

    BoardUI* board_ui;
    TreeUI* tree_ui;

    Window* ui_buttons;
    Window* ui_rules;
    Window* ui_log;
    Window* ui_help;
    Window* ui_properties;
    Window* ui_player_info;
    Window* ui_mini_tree;

    Window* ui_upgrade_description;

    MenuPauseScreen screen_pause;
    Menu* ui_pause_main;
    Menu* ui_pause_options;
    Menu* ui_pause_exit;

    TTF_Font* ui_font_small;
    TTF_Font* ui_font_medium;
    TTF_Font* ui_font_large;

    SDL_Color* ui_text_color;

    Sound* sound_click;
    Sound* sound_error;

    float scale;

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
static Window* create_ui_rules(SDL_Renderer* renderer, GameUI* ui);
static void update_ui_rules(SDL_Renderer* renderer, Window* window, const GameUI* ui);
static Window* create_ui_log(SDL_Renderer* renderer, GameUI* ui);
static void update_ui_log(SDL_Renderer* renderer, Window* window, const GameUI* ui);
static Window* create_ui_help(SDL_Renderer* renderer, GameUI* ui);

static Window* create_ui_upgrade_description(SDL_Renderer* renderer, GameUI* ui);
static void update_ui_upgrade_description(SDL_Renderer* renderer, Window* window, GameUI* ui);

static Menu* create_ui_pause_main(SDL_Renderer* renderer, GameUI* ui);
static Menu* create_ui_pause_options(SDL_Renderer* renderer, GameUI* ui);
static Menu* create_ui_pause_exit(SDL_Renderer* renderer, GameUI* ui);

static void window_set_minimap(GameUI* ui, Window* window);
static void window_set_fullscreen(GameUI* ui, Window* window);

static void game_ui_purchase_upgrade(void* game_ui, void* index);

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

    window_set_minimap(ui, tree_ui_get_window(ui->tree_ui));

    ui->screen = SCREEN_BOARD;
    ui->state = SDL_malloc(sizeof(UIState));
    *ui->state = (UIState)
        {
            false, 
            false, 
            false, 
            false, 
            false,
            false,
            false
        };

    ui->ui_font_small = TTF_OpenFont(FONT_PATH, FONT_SIZE * (g_app_scale - 0.4));
    ui->ui_font_medium = TTF_OpenFont(FONT_PATH, FONT_SIZE * (g_app_scale));
    ui->ui_font_large = TTF_OpenFont(FONT_PATH, FONT_SIZE * (g_app_scale + 0.1));

    ui->ui_text_color = SDL_malloc(sizeof(SDL_Color));
    *ui->ui_text_color = color_white();

    ui->sound_click = sound_load(PATH_SOUND_CLICK);
    ui->sound_error = sound_load(PATH_SOUND_ERROR);

    ui->ui_buttons = create_ui_buttons(renderer, ui);
    ui->ui_rules = create_ui_rules(renderer, ui);
    ui->ui_log = create_ui_log(renderer, ui);
    ui->ui_help = create_ui_help(renderer, ui);
    ui->ui_player_info = create_ui_player_info(renderer);
    ui->ui_properties = create_ui_properties(renderer);

    ui->ui_upgrade_description = create_ui_upgrade_description(renderer, ui);

    ui->ui_pause_main = create_ui_pause_main(renderer, ui);
    ui->ui_pause_options = create_ui_pause_options(renderer, ui);
    ui->ui_pause_exit = create_ui_pause_exit(renderer, ui);

    return ui;
}

void game_ui_destroy(GameUI* ui)
{
    verify_game_ui(ui);

    board_ui_destroy(ui->board_ui);
    tree_ui_destroy(ui->tree_ui);

    SDL_free(ui->state);

    window_destroy(ui->ui_buttons);
    window_destroy(ui->ui_rules);
    window_destroy(ui->ui_log);
    window_destroy(ui->ui_help);
    window_destroy(ui->ui_player_info);
    window_destroy(ui->ui_properties);

    window_destroy(ui->ui_upgrade_description);

    menu_destroy(ui->ui_pause_main);
    menu_destroy(ui->ui_pause_options);
    menu_destroy(ui->ui_pause_exit);

    sound_unload(ui->sound_click);
    sound_unload(ui->sound_error);

    TTF_CloseFont(ui->ui_font_small);
    TTF_CloseFont(ui->ui_font_medium);
    TTF_CloseFont(ui->ui_font_large);

    SDL_free(ui->ui_text_color);

    SDL_free(ui);
}

static void draw_border(SDL_Renderer* renderer, SDL_Rect rect, SDL_Color color)
{
    SDL_FRect frect;
    SDL_RectToFRect(&rect, &frect);
    SDL_SetRenderClipRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderRect(renderer, &frect);
}

void game_ui_render(SDL_Renderer* renderer, GameUI* ui)
{
    verify_renderer(renderer);
    verify_game_ui(ui);

    if (ui->screen == SCREEN_BOARD)
    {
        board_ui_render(renderer, ui->board_ui);
        tree_ui_render(renderer, ui->tree_ui);
        update_ui_properties(renderer, ui->ui_properties, ui);
        if (ui->state->show_properties)
        {
            window_render(renderer, ui->ui_properties);
        }
    }
    if (ui->screen == SCREEN_TREE)
    {
        tree_ui_render(renderer, ui->tree_ui);
        board_ui_render(renderer, ui->board_ui);
        update_ui_upgrade_description(renderer, ui->ui_upgrade_description, ui);
        if (ui->state->show_upgrade_description)
        {
            window_render(renderer, ui->ui_upgrade_description);
        }
    }

    window_render(renderer, ui->ui_buttons);

    update_ui_player_info(renderer, ui->ui_player_info, ui);
    window_render(renderer, ui->ui_player_info);

    if (ui->state->show_rules)
    {
        update_ui_rules(renderer, ui->ui_rules, ui);
        window_render(renderer, ui->ui_rules);
    }

    if (ui->state->show_log)
    {
        update_ui_log(renderer, ui->ui_log, ui);
        window_render(renderer, ui->ui_log);
    }

    if (ui->state->show_help)
    {
        window_render(renderer, ui->ui_help);
    }

    if (ui->state->is_paused)
    {
        if (ui->screen_pause == MENU_PAUSE_MAIN)
        {
            menu_render(renderer, ui->ui_pause_main);
        }
        else if (ui->screen_pause == MENU_PAUSE_OPTIONS)
        {
            menu_render(renderer, ui->ui_pause_options);
        }
        else if (ui->screen_pause == MENU_PAUSE_EXIT)
        {
            menu_render(renderer, ui->ui_pause_exit);
        }
    }
    draw_border(
        renderer, 
        (SDL_Rect)
            {
                screen_width - UI_MINI_MAP_WIDTH - UI_BUFFER,
                screen_height - UI_MINI_MAP_HEIGHT - UI_BUFFER,
                UI_MINI_MAP_WIDTH,
                UI_MINI_MAP_HEIGHT
            },
        (SDL_Color) {0, 0, 0, 255}
    );
}

static void update_keys_minimap(const InputState* input, GameUI* ui)
{
    float mouse_x = mouse_get_x(input);
    float mouse_y = mouse_get_y(input);
    if (mouse_x < screen_width - UI_MINI_MAP_WIDTH - UI_BUFFER) return;
    if (mouse_y < screen_height - UI_MINI_MAP_HEIGHT - UI_BUFFER) return;
    if (mouse_x > screen_width - UI_BUFFER) return;
    if (mouse_y > screen_height - UI_BUFFER) return;
    if (mouse_get_released(input, MOUSE_LEFT))
    {
        toggle_screen(ui);
    }
}

static void update_keys(const InputState* input, GameUI* ui)
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
        if (ui->screen == SCREEN_BOARD)
        {
            if (board_ui_has_selected_pos(ui->board_ui))
            {
                ui->state->show_properties = false;
                deselect_pos(ui->board_ui);
            }
        }
        else if (ui->screen == SCREEN_TREE)
        {
            if (tree_ui_has_selected_upgrade(ui->tree_ui))
            {
                ui->state->show_upgrade_description = false;
                tree_ui_deselect_upgrade(ui->tree_ui);
            }
        }
    }
}

GameResult game_ui_update(InputState* input, GameUI* ui)
{
    verify_game_ui(ui);
    verify_input(input);

    window_update(input, ui->ui_buttons);

    Board* board = game_get_board(ui->game);
    Color player_color = player_get_color(game_get_opponent(ui->game));
    if (is_check_mate(board, player_color))
    {
        return player_color == WHITE ? GAME_RESULT_WIN_WHITE : GAME_RESULT_WIN_BLACK;
    }
    if (ui->state->exit_game) return GAME_RESULT_EXIT;

    update_keys(input, ui);

    if (ui->state->is_paused)
    {
        if (ui->screen_pause == MENU_PAUSE_MAIN)
        {
            menu_update(input, ui->ui_pause_main);
        }
        else if (ui->screen_pause == MENU_PAUSE_OPTIONS)
        {
            menu_update(input, ui->ui_pause_options);
        }
        else if (ui->screen_pause == MENU_PAUSE_EXIT)
        {
            menu_update(input, ui->ui_pause_exit);
        }
        return GAME_RESULT_CONTINUE;
    }

    update_keys_minimap(input, ui);

    if (ui->screen == SCREEN_BOARD)
    {
        board_ui_update(input, ui->board_ui);
        if (ui->state->show_properties)
        {
            window_update(input, ui->ui_properties);
        }
        if (ui->state->show_rules)
        {
            window_update(input, ui->ui_rules);
        }
        if (ui->state->show_log)
        {
            window_update(input, ui->ui_log);
        }
        if (ui->state->show_help)
        {
            window_update(input, ui->ui_help);
        }
        return GAME_RESULT_CONTINUE;
    }
    if (ui->screen == SCREEN_TREE)
    {
        tree_ui_update(input, ui->tree_ui);
        if (ui->state->show_upgrade_description)
        {
            window_update(input, ui->ui_upgrade_description);
        }
        return GAME_RESULT_CONTINUE;
    }

    return GAME_RESULT_CONTINUE;
}

static void toggle_screen(GameUI* ui)
{
    // TODO: fix centering
    if (ui->screen == SCREEN_BOARD)
    {
        ui->screen = SCREEN_TREE;
        Window* tree = tree_ui_get_window(ui->tree_ui);
        window_set_fullscreen(ui, tree);

        Window* board = board_ui_get_window(ui->board_ui);
        window_set_minimap(ui, board);
        return;
    }
    if (ui->screen == SCREEN_TREE)
    {
        ui->screen = SCREEN_BOARD;
        Window* board = board_ui_get_window(ui->board_ui);
        window_set_fullscreen(ui, board);

        Window* tree = tree_ui_get_window(ui->tree_ui);
        window_set_minimap(ui, tree);
        board_ui_update_objects(ui->board_ui);
        return;
    }
}

static void window_set_minimap(GameUI* ui, Window* window)
{
    ui->scale = window_get_scale(window);
    window_set_size(
        window,
        UI_MINI_MAP_WIDTH,
        UI_MINI_MAP_HEIGHT
    );
    window_set_position(
        window,
        screen_width - UI_MINI_MAP_WIDTH - UI_BUFFER,
        screen_height - UI_MINI_MAP_HEIGHT - UI_BUFFER
    );
    window_set_scale(window, g_app_scale * 2);
}

static void window_set_fullscreen(GameUI* ui, Window* window)
{
    window_set_scale(window, ui->scale);
    window_set_size(window, screen_width, screen_height);
    window_set_position(window, 0, 0);
}

static void toggle_pause(void* game_ui, void* null)
{
    GameUI* ui = (GameUI*)game_ui;
    ui->state->is_paused = !ui->state->is_paused;
    ui->state->show_rules = false;
    ui->state->show_log = false;
    ui->state->show_help = false;
    ui->screen_pause = MENU_PAUSE_MAIN;
    sound_play(ui->sound_click);

}
static void toggle_rules(void* game_ui, void* null)
{
    GameUI* ui = (GameUI*)game_ui;
    ui->state->is_paused = false;
    ui->state->show_rules = !ui->state->show_rules;
    ui->state->show_log = false;
    ui->state->show_help = false;
    sound_play(ui->sound_click);
}
static void toggle_log(void* game_ui, void* null)
{
    GameUI* ui = (GameUI*)game_ui;
    ui->state->is_paused = false;
    ui->state->show_rules = false;
    ui->state->show_log = !ui->state->show_log;
    ui->state->show_help = false;
    sound_play(ui->sound_click);
}
static void toggle_help(void* game_ui, void* null)
{
    GameUI* ui = (GameUI*)game_ui;
    ui->state->is_paused = false;
    ui->state->show_rules = false;
    ui->state->show_log = false;
    ui->state->show_help = !ui->state->show_help;
    sound_play(ui->sound_click);
}
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

static Window* create_ui_rules(SDL_Renderer* renderer, GameUI* ui)
{
    Window* window = window_create(
        screen_width - UI_RULES_WIDTH - UI_BUFFER,
        UI_BUFFER * 2 + UI_BUTTON_SIZE,
        UI_RULES_WIDTH,
        UI_RULES_HEIGHT,
        SDL_CreateTextureFromPNG(renderer, PATH_TEXTURE_WINDOW_RULES),
        0
    );

    Textbox* title = textbox_create(
        renderer,
        ui->ui_font_small,
        ui->ui_text_color,
        "RULES",
        window_get_width(window) - UI_BUFFER * 2,
        TEXT_LEFT_ALIGNED
    );
    window_add_textbox(window, title, UI_BUFFER, UI_BUFFER);

    Textbox* rules = textbox_create(
        renderer,
        ui->ui_font_small,
        ui->ui_text_color,
        " ",
        window_get_width(window) - UI_BUFFER * 2,
        TEXT_LEFT_ALIGNED
    );
    window_add_textbox(window, rules, UI_BUFFER, UI_BUFFER * 2);

    return window;
}

static void update_ui_rules(SDL_Renderer* renderer, Window* window, const GameUI* ui)
{
    Vector* textboxes = window_get_textboxes(window);

    Textbox* rules = vector_get_at(textboxes, 1);
    const char* text = rulelist_get_rules(game_get_rules(ui->game));
    textbox_set_text(renderer, rules, text);
}

static Window* create_ui_log(SDL_Renderer* renderer, GameUI* ui)
{
    Window* window = window_create(
        screen_width - UI_LOG_WIDTH - UI_BUFFER,
        UI_BUFFER * 2 + UI_BUTTON_SIZE,
        UI_LOG_WIDTH,
        UI_LOG_HEIGHT,
        SDL_CreateTextureFromPNG(renderer, PATH_TEXTURE_WINDOW_LOG),
        0
    );

    Textbox* title = textbox_create(
        renderer,
        ui->ui_font_small,
        ui->ui_text_color,
        "HISTORY",
        window_get_width(window) - UI_BUFFER * 2,
        TEXT_LEFT_ALIGNED
    );
    window_add_textbox(window, title, UI_BUFFER, UI_BUFFER);

    Textbox* textbox = textbox_create(
        renderer, 
        ui->ui_font_small, 
        ui->ui_text_color, 
        " ", 
        UI_LOG_WIDTH - UI_BUFFER * 2, 
        TEXT_LEFT_ALIGNED
    );
    window_add_textbox(
        window,
        textbox,
        UI_BUFFER,
        UI_LOG_HEIGHT - textbox_get_height(textbox) - UI_BUFFER
    );

    return window;
}

static void update_ui_log(SDL_Renderer* renderer, Window* window, const GameUI* ui)
{
    Vector* textboxes = window_get_textboxes(window);

    Textbox* log = vector_get_at(textboxes, 1);
    const char* text = gamelog_get(game_get_log(ui->game));
    textbox_set_text(renderer, log, text);
}

static void menu_pause_set_main(void* game_ui, void* null)
{
    GameUI* ui = game_ui;
    ui->screen_pause = MENU_PAUSE_MAIN;
    sound_play(ui->sound_click);
}

static void menu_pause_set_options(void* game_ui, void* null)
{
    GameUI* ui = game_ui;
    ui->screen_pause = MENU_PAUSE_OPTIONS;
    sound_play(ui->sound_click);
}

static void menu_pause_set_exit(void* game_ui, void* null)
{
    GameUI* ui = game_ui;
    ui->screen_pause = MENU_PAUSE_EXIT;
    sound_play(ui->sound_click);
}

static void end_game(void* game_ui, void* null)
{
    GameUI* ui = game_ui;
    ui->state->exit_game = true;
}

static Menu* create_ui_pause_main(SDL_Renderer* renderer, GameUI* ui)
{
    Menu* menu = menu_create(
        renderer,
        screen_width - UI_PAUSE_WIDTH - UI_BUFFER,
        UI_BUFFER * 2 + UI_BUTTON_SIZE,
        UI_PAUSE_WIDTH,
        UI_PAUSE_HEIGHT
    );
    Function* func;
    func = function_create(toggle_pause, ui, NULL);
    menu_add_button(renderer, menu, func, "CONTINUE");
    func = function_create(menu_pause_set_options, ui, NULL),
    menu_add_button(renderer, menu, func, "OPTIONS");
    func = function_create(menu_pause_set_exit, ui, NULL),
    menu_add_button(renderer, menu, func, "EXIT TO MAIN MENU");
    return menu;
}

static void toggle_sound(void* game_ui, void* null2)
{
    GameUI* ui = game_ui;
    g_volume_on = !g_volume_on;
    sound_play(ui->sound_click);
}

static Menu* create_ui_pause_options(SDL_Renderer* renderer, GameUI* ui)
{
    Menu* menu = menu_create(
        renderer,
        screen_width - UI_PAUSE_WIDTH - UI_BUFFER,
        UI_BUFFER * 2 + UI_BUTTON_SIZE,
        UI_PAUSE_WIDTH,
        UI_PAUSE_HEIGHT
    );
    Function* func;
    func = function_create(toggle_sound, ui, NULL);
    menu_add_button(renderer, menu, func,"SOUND");
    func = function_create(menu_pause_set_main, ui, NULL);
    menu_add_button(renderer, menu, func,"BACK");
    return menu;
}

static Menu* create_ui_pause_exit(SDL_Renderer* renderer, GameUI* ui)
{
    Menu* menu = menu_create(
        renderer,
        screen_width - UI_PAUSE_WIDTH - UI_BUFFER,
        UI_BUFFER * 2 + UI_BUTTON_SIZE,
        UI_PAUSE_WIDTH,
        UI_PAUSE_HEIGHT
    );
    Function* func;
    func = function_create(end_game, ui, NULL);
    menu_add_button(renderer, menu, func, "CONFIRM");
    func = function_create(menu_pause_set_main, ui, NULL);
    menu_add_button(renderer, menu, func, "BACK");
    return menu;
}

static Window* create_ui_upgrade_description(SDL_Renderer* renderer, GameUI* ui)
{
    Window* window = window_create(
        UI_BUFFER,
        screen_height - UI_UPGRADE_DESCRIPTION_HEIGHT - UI_BUFFER,
        UI_UPGRADE_DESCRIPTION_WIDTH,
        UI_UPGRADE_DESCRIPTION_HEIGHT,
        SDL_CreateTextureFromPNG(renderer, PATH_TEXTURE_WINDOW_UPGRADE_DESCRIPTION),
        WINDOW_SCROLLABLE
    );
    window_load_textures(renderer, window, ui_buttons_textures, UI_BUTTONS_TEXTURE_COUNT);

    Textbox* textbox = textbox_create(
        renderer,
        ui->ui_font_small,
        ui->ui_text_color,
        " ",
        window_get_width(window) - UI_BUFFER * 2,
        TEXT_LEFT_ALIGNED
    );
    window_add_textbox(window, textbox, UI_BUFFER, UI_BUFFER);

    SDL_Texture* texture = window_get_texture(window, TEXTURE_BUTTON_PAUSE);
    Button* button = button_create(texture, NULL, NULL);
    button_set_size(
        button,
        UI_UPGRADE_DESCRIPTION_BUTTON_WIDTH,
        UI_UPGRADE_DESCRIPTION_BUTTON_HEIGHT
    );
    window_add_button(
        window, 
        button,
        UI_BUFFER,
        window_get_height(window) - UI_UPGRADE_DESCRIPTION_BUTTON_HEIGHT - UI_BUFFER
    );

    Textbox* purchase = textbox_create(
        renderer, 
        ui->ui_font_small, 
        ui->ui_text_color, 
        "Purchase", 
        window_get_width(window), 
        TEXT_CENTERED
    );
    window_add_textbox(
        window, 
        purchase, 
        window_get_width(window) / 2,
        window_get_height(window) - UI_UPGRADE_DESCRIPTION_BUTTON_HEIGHT - UI_BUFFER + UI_UPGRADE_DESCRIPTION_BUTTON_HEIGHT / 2 - textbox_get_height(purchase) / 2
    );

    return window;
}

static void update_ui_upgrade_description(SDL_Renderer* renderer, Window* window, GameUI* ui)
{
    Tree* tree = game_get_tree(ui->game);

    if (!tree_ui_has_selected_upgrade(ui->tree_ui))
    {
        ui->state->show_upgrade_description = false;
        return;
    }

    ui->state->show_upgrade_description = true;

    int index = tree_ui_get_selected_upgrade(ui->tree_ui);

    Vector* textboxes = window_get_textboxes(window);
    Textbox* description = vector_get_at(textboxes, 0);
    const char* text = get_upgrade_properties(tree, index);
    textbox_set_text(renderer, description, text);

    Vector* buttons = window_get_buttons(window);
    Button* button = vector_get_at(buttons, 0);
    int* i = SDL_malloc(sizeof(int));
    *i = index;
    button_set_on_click_fn(
        button,
        MOUSE_LEFT,
        function_create(game_ui_purchase_upgrade, ui, i)
    );
}

static Window* create_ui_help(SDL_Renderer* renderer, GameUI* ui)
{
    Window* window = window_create(
        screen_width - UI_LOG_WIDTH - UI_BUFFER,
        UI_BUFFER * 2 + UI_BUTTON_SIZE,
        UI_LOG_WIDTH,
        UI_LOG_HEIGHT,
        SDL_CreateTextureFromPNG(renderer, PATH_TEXTURE_WINDOW_LOG),
        0
    );

    Textbox* title = textbox_create(
        renderer,
        ui->ui_font_small,
        ui->ui_text_color,
        "HELP",
        window_get_width(window) - UI_BUFFER * 2,
        TEXT_LEFT_ALIGNED
    );
    window_add_textbox(window, title, UI_BUFFER, UI_BUFFER);

    Textbox* help = textbox_create(
        renderer, 
        ui->ui_font_small,
        ui->ui_text_color,
        DESCRIPTION_HELP,
        window_get_width(window) - UI_BUFFER * 2,
        TEXT_LEFT_ALIGNED
    );
    window_add_textbox(window, help, UI_BUFFER, UI_BUFFER * 2 + textbox_get_height(title));

    return window;
}


static void game_ui_purchase_upgrade(void* game_ui, void* index)
{
    GameUI* ui = game_ui;
    int i = *(int*)index;
    if (!game_purchase_upgrade(ui->game, i))
    {
        sound_play(ui->sound_error);
        return;
    }
    board_ui_update_objects(ui->board_ui);
    toggle_screen(ui);
    sound_play(ui->sound_click);
}

void verify_game_ui(const GameUI* ui)
{
    verify(ui == NULL, "GameUI does not exist");
}
