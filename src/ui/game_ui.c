#include "ui/game_ui.h"
#include "helper_functions/function.h"
#include "helper_functions/helper_functions.h"
#include "inputstate.h"
#include "ui/textures/ui_textures.h"
#include "ui/ui_elements/button.h"
#include "ui/ui_elements/window.h"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>

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

    GameScreen screen;
    UIState* state;
};

static void toggle_screen(GameUI* ui);
static void toggle_pause(void* game_ui, void* null);

static Window* create_ui_buttons(SDL_Renderer* renderer, GameUI* ui);
static Window* create_ui_properties(SDL_Renderer* renderer, GameUI* ui);

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
    *ui->state = (UIState){false, true, false, false};

    ui->ui_buttons = create_ui_buttons(renderer, ui);
    ui->ui_properties = create_ui_properties(renderer, ui);

    return ui;
}

void game_ui_destroy(GameUI* ui)
{
    verify_game_ui(ui);

    board_ui_destroy(ui->board_ui);
    tree_ui_destroy(ui->tree_ui);
}

void game_ui_render(SDL_Renderer* renderer, const GameUI* ui)
{
    verify_renderer(renderer);
    verify_game_ui(ui);

    if (ui->screen == SCREEN_BOARD)
    {
        board_ui_render(renderer, ui->board_ui);
        window_render(renderer, ui->ui_buttons);
        if (ui->state->show_properties)
        {
            window_render(renderer, ui->ui_properties);
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

    float window_width = button_count * UI_BUTTON_SIZE + (button_count - 1) * UI_BUTTON_BUFFER;
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
        window_add_button(window, button, i * (UI_BUTTON_SIZE + UI_BUTTON_BUFFER), 0);
    }

    return window;
}

static Window* create_ui_properties(SDL_Renderer* renderer, GameUI* ui)
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
void update_ui_properties(Window* window);

Window* create_ui_player_info(Window* window);
void update_ui_player_info(Window* window);

Window* create_ui_rules();
void update_ui_rules(Window* window);

Window* create_ui_mini_tree();
void update_ui_mini_tree(Window* window);

Window* create_ui_pause();







void verify_game_ui(const GameUI* ui)
{
    verify(ui == NULL, "GameUI does not exist");
}
