#include "game/ui/game_ui.h"
#include <SDL3/SDL_scancode.h>

typedef enum GameScreen
{
    GAME_SCREEN_BOARD,
    GAME_SCREEN_TREE,
} GameScreen;

struct GameUI 
{
    Game* game;

    BoardUI* board_ui;
    TreeUI* tree_ui;
    GameScreen screen;

    // player info
    // properties
    // rules
    // buttons
    Menu* menu;

    SDL_Renderer* renderer;

    bool is_paused;
    bool end_game;
};

static void menu_set_pause_main(void* game_ui, void* null);
static void menu_set_pause_options(void* game_ui, void* null);
static void menu_set_pause_quit(void* game_ui, void* null);

static void toggle_pause(void* game_ui, void* null);
static void toggle_screen(void* game_ui, void* null);

GameUI* game_ui_create(Game* game, SDL_Renderer* renderer)
{
    GameUI* ui = SDL_malloc(sizeof(GameUI));
    verify(ui == NULL, "GameUI could not be created: malloc");

    ui->game = game;

    ui->board_ui = board_ui_create(
        renderer,
        game_get_board(ui->game),
        0,
        0,
        g_app_window_width,
        g_app_window_height
    );
    ui->tree_ui = tree_ui_create(
        renderer,
        game_get_tree(ui->game),
        ui->board_ui,
        0,
        0,
        g_app_window_width,
        g_app_window_height
    );
    ui->screen = GAME_SCREEN_BOARD;

    ui->menu = NULL;

    ui->renderer = renderer;

    ui->is_paused = false;
    ui->end_game = false;

    return ui;
}

void game_ui_destroy(GameUI* ui)
{
    verify_game_ui(ui);

    board_ui_destroy(ui->board_ui);
    tree_ui_destroy(ui->tree_ui);
    if (ui->menu != NULL) menu_destroy(ui->menu);
}

void game_ui_render(SDL_Renderer* renderer, const GameUI* ui)
{
    verify_renderer(renderer);
    verify_game_ui(ui);

    if (ui->screen == GAME_SCREEN_BOARD)
    {
        board_ui_render(renderer, ui->board_ui);
    }
    if (ui->screen == GAME_SCREEN_TREE)
    {
        tree_ui_render(renderer, ui->tree_ui);
    }

    if (ui->menu == NULL)
    {
        return;
    }
    menu_render(renderer, ui->menu);
}

static void update_keys(InputState* input, GameUI* ui)
{
    if (keyboard_get_pressed(input, SDL_SCANCODE_ESCAPE))
    {
        game_ui_toggle_pause(ui);
    }
    if (keyboard_get_pressed(input, SDL_SCANCODE_TAB))
    {
        toggle_screen(ui, NULL);
    }
}
int game_ui_update(InputState* input, GameUI* ui)
{
    verify_game_ui(ui);
    verify_input(input);

    update_keys(input, ui);

    if (ui->end_game) 
    {
        return 1;
    }
    if (ui->is_paused)
    {
        menu_update(input, ui->menu);
        return 0;
    }
    if (ui->screen == GAME_SCREEN_BOARD)
    {
        board_ui_update(input, ui->board_ui);
    }
    if (ui->screen == GAME_SCREEN_TREE)
    {
        tree_ui_update(input, ui->tree_ui);
    }
    return 0;
}

static void toggle_pause(void* game_ui, void* null)
{
    (void)null;
    game_ui_toggle_pause(game_ui);
}
static void toggle_screen(void* game_ui, void* null)
{
    (void)null;
    GameUI* ui = game_ui;
    if (ui->screen == GAME_SCREEN_BOARD) 
    {
        ui->screen = GAME_SCREEN_TREE;
        return;
    }
    if (ui->screen == GAME_SCREEN_TREE)
    {
        ui->screen = GAME_SCREEN_BOARD;
    }
}
static void menu_set_pause_main(void* game_ui, void* null)
{
    (void)null;
    GameUI* ui = (GameUI*)game_ui;
    Function* func;

    if (ui->menu != NULL)
    {
        menu_destroy(ui->menu);
    }

    ui->menu = menu_create(
        ui->renderer,
        (float)g_app_window_width / 4,
        (float)g_app_window_height / 4,
        (float)g_app_window_width / 2,
        (float)g_app_window_height / 2
    );

    // CONTINUE
    func = function_create(toggle_pause, ui, NULL);
    menu_add_button(ui->renderer, ui->menu, func, "CONTINUE");
    // OPTIONS
    func = function_create(menu_set_pause_options, ui, NULL);
    menu_add_button(ui->renderer, ui->menu, func, "OPTIONS");
    // QUIT TO MAIN MENU
    func = function_create(menu_set_pause_quit, ui, NULL);
    menu_add_button(ui->renderer, ui->menu, func, "QUIT TO MAIN MENU");
}

static void menu_set_pause_options(void* game_ui, void* null)
{
    (void)null;
    GameUI* ui = (GameUI*)game_ui;
    Function* func;

    if (ui->menu != NULL)
    {
        menu_destroy(ui->menu);
    }

    ui->menu = menu_create(
        ui->renderer,
        (float)g_app_window_width / 4,
        (float)g_app_window_height / 4,
        (float)g_app_window_width / 2,
        (float)g_app_window_height / 2
    );

    // BACK
    func = function_create(menu_set_pause_main, ui, NULL);
    menu_add_button(ui->renderer, ui->menu, func, "BACK");
}
static void menu_set_pause_quit(void* game_ui, void* null)
{
    (void)null;
    GameUI* ui = (GameUI*)game_ui;
    Function* func;

    if (ui->menu != NULL)
    {
        menu_destroy(ui->menu);
    }

    ui->menu = menu_create(
        ui->renderer,
        (float)g_app_window_width / 4,
        (float)g_app_window_height / 4,
        (float)g_app_window_width / 2,
        (float)g_app_window_height / 2
    );

    // QUIT TO MAIN MENU
    func = NULL; // TODO:
    menu_add_button(ui->renderer, ui->menu, func, "QUIT TO MAIN MENU");
    // BACK
    func = function_create(menu_set_pause_main, ui, NULL);
    menu_add_button(ui->renderer, ui->menu, func, "BACK");
}


void game_ui_toggle_pause(GameUI* ui)
{
    verify_game_ui(ui);

    ui->is_paused = !ui->is_paused;

    if (ui->is_paused)
    {
        menu_set_pause_main(ui, NULL);
        return;
    }
    if (!ui->is_paused)
    {
        menu_destroy(ui->menu);
        ui->menu = NULL;
        return;
    }
}



void verify_game_ui(const GameUI* ui)
{
    verify(ui == NULL, "GameUI does not exist");
}
