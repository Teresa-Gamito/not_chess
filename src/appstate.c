#include "include/appstate.h"

typedef struct AppMenu
{
    Menu* start;
    Menu* main;
    Menu* options;
    Menu* credits;
    Menu* instructions;
    Menu* exit;
} AppMenu;

typedef enum Screen
{
    SCREEN_GAME,
    SCREEN_MENU_START,
    SCREEN_MENU_MAIN,
    SCREEN_MENU_OPTIONS,
    SCREEN_MENU_CREDITS,
    SCREEN_MENU_INSTRUCTIONS,
    SCREEN_MENU_EXIT
} Screen;

struct AppState
{
    SDL_Window* sdl_window;
    SDL_Renderer* sdl_renderer;

    InputState* input;

    AppMenu* menu;

    Game* game;
    GameUI* game_ui;

    Screen screen;
};

static Menu* create_menu_start(SDL_Renderer* renderer, AppState* app);
static Menu* create_menu_main(SDL_Renderer* renderer, AppState* app);
static Menu* create_menu_options(SDL_Renderer* renderer, AppState* app);
static Menu* create_menu_credits(SDL_Renderer* renderer, AppState* app);
static Menu* create_menu_exit(SDL_Renderer* renderer, AppState* app);
static Menu* create_menu_instructions(SDL_Renderer* renderer, AppState* app);
static void start_game(void* app_state, void* null);

AppState* app_create()
{
    AppState* app = (AppState*)SDL_malloc(sizeof(AppState));
    verify(app == NULL, "AppState could not be created: malloc");

    TTF_Init();

    app->sdl_window = SDL_CreateWindow(
        APP_NAME,
        APP_WINDOW_WIDTH,
        APP_WINDOW_HEIGHT,
        APP_WINDOW_FLAGS
    );
    verify(app->sdl_window == NULL, "SDL_Window could not be created");

    app->sdl_renderer = SDL_CreateRenderer(app->sdl_window, NULL);
    verify(app->sdl_renderer == NULL, "SDL_Renderer could not be created");

    SDL_SetDefaultTextureScaleMode(app->sdl_renderer, SDL_SCALEMODE_PIXELART);

    app->game = NULL;
    app->game_ui = NULL;

    app->menu = SDL_malloc(sizeof(AppMenu));
    app->menu->start = create_menu_start(app->sdl_renderer, app);
    app->menu->main = create_menu_main(app->sdl_renderer, app);
    app->menu->options = create_menu_options(app->sdl_renderer, app);
    app->menu->credits = create_menu_credits(app->sdl_renderer, app);
    app->menu->instructions = create_menu_instructions(app->sdl_renderer, app);
    app->menu->exit = create_menu_exit(app->sdl_renderer, app);

    app->screen = SCREEN_MENU_START;

    app->input = input_create();

    return app;
}

void app_destroy(AppState* app)
{
    verify_app_state(app);

    SDL_DestroyWindow(app->sdl_window);
    SDL_DestroyRenderer(app->sdl_renderer);

    menu_destroy(app->menu->start);
    menu_destroy(app->menu->main);
    menu_destroy(app->menu->options);
    menu_destroy(app->menu->credits);
    menu_destroy(app->menu->instructions);
    menu_destroy(app->menu->exit);
    SDL_free(app->menu);

    input_destroy(app->input);

    if (app->screen == SCREEN_GAME)
    {
        game_ui_destroy(app->game_ui);
        game_destroy(app->game);
    }

    TTF_Quit();

    SDL_free(app);
}

static void update_global_variables(AppState* app)
{
    int app_width;
    int app_height;
    SDL_GetWindowSize(app->sdl_window, &app_width, &app_height);
    g_app_window_width = app_width;
    g_app_window_height = app_height;
    if ((float) APP_WINDOW_WIDTH / APP_WINDOW_HEIGHT < g_app_window_width / g_app_window_height)
    {
        g_app_scale = g_app_window_width / APP_WINDOW_WIDTH;
    }
    else
    {
        g_app_scale = g_app_window_height / APP_WINDOW_HEIGHT;
    }
    g_menu_font_size = FONT_SIZE_MENU * g_app_scale;
    g_game_font_size = FONT_SIZE * g_app_scale;
}

static void update_cheats(AppState* app)
{
    InputState* input = app->input;

    if (keyboard_get_pressed(input, SDL_SCANCODE_1))
    {
        debug_infinite_points = !debug_infinite_points;
    }
    if (keyboard_get_pressed(input, SDL_SCANCODE_2))
    {
        debug_can_purchace_multiple_times = !debug_can_purchace_multiple_times;
    }
}

void app_update(AppState* app)
{
    verify_app_state(app);

    InputState* input = app->input;
    GameResult result = GAME_RESULT_CONTINUE;

    switch (app->screen)
    {
        case SCREEN_MENU_START:
            menu_update(input, app->menu->start);
            return;
        case SCREEN_MENU_MAIN:
            menu_update(input, app->menu->main);
            return;
        case SCREEN_MENU_OPTIONS:
            menu_update(input, app->menu->options);
            return;
        case SCREEN_MENU_CREDITS:
            menu_update(input, app->menu->credits);
            return;
        case SCREEN_MENU_EXIT:
            menu_update(input, app->menu->exit);
            return;
        case SCREEN_MENU_INSTRUCTIONS:
            menu_update(input, app->menu->instructions);
            return;
        case SCREEN_GAME:
            result = game_ui_update(input, app->game_ui);
            break;
    }

    switch (result)
    {
        case GAME_RESULT_CONTINUE:
            break;

        case GAME_RESULT_WIN_WHITE:
            SDL_Log("WHITE WINS!");
            app_quit(NULL, NULL);
            break;

        case GAME_RESULT_WIN_BLACK:
            SDL_Log("BLACK WINS!");
            app_quit(NULL, NULL);
            break;

        case GAME_RESULT_EXIT:
            app->screen = SCREEN_MENU_MAIN;
            game_ui_destroy(app->game_ui);
            game_destroy(app->game);
            break;
    }

    update_global_variables(app);
    update_cheats(app);
}

void app_render(AppState* app)
{
    verify_app_state(app);

    SDL_Renderer* renderer = app->sdl_renderer;

    SDL_SetRenderDrawColor(renderer, 12, 23, 34, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    switch (app->screen)
    {
        case SCREEN_MENU_START:
            menu_render(renderer, app->menu->start);
            break;
        case SCREEN_MENU_MAIN:
            menu_render(renderer, app->menu->main);
            break;
        case SCREEN_MENU_OPTIONS:
            menu_render(renderer, app->menu->options);
            break;
        case SCREEN_MENU_CREDITS:
            menu_render(renderer, app->menu->credits);
            break;
        case SCREEN_MENU_EXIT:
            menu_render(renderer, app->menu->exit);
            break;
        case SCREEN_MENU_INSTRUCTIONS:
            menu_render(renderer, app->menu->instructions);
            break;
        case SCREEN_GAME:
            game_ui_render(renderer, app->game_ui);
            break;
    }

    SDL_RenderPresent(renderer);
}

InputState* app_get_inputstate(const AppState* app)
{
    verify_app_state(app);

    return app->input;
}

static void start_game(void* app_state, void* null)
{
    AppState* app = app_state;
    app->screen = SCREEN_GAME;

    app->game = game_create();
    game_start(app->game);

    app->game_ui = game_ui_create(app->game, app->sdl_renderer);
}

static void menu_set_main(void* app_state, void* null)
{
    AppState* app = app_state;
    app->screen = SCREEN_MENU_MAIN;
}

static void menu_set_options(void* app_state, void* null)
{
    AppState* app = app_state;
    app->screen = SCREEN_MENU_OPTIONS;
}

static void menu_set_credits(void* app_state, void* null)
{
    AppState* app = app_state;
    app->screen = SCREEN_MENU_CREDITS;
}

static void menu_set_instructions(void* app_state, void* null)
{
    AppState* app = app_state;
    app->screen = SCREEN_MENU_INSTRUCTIONS;
}

static void menu_set_exit(void* app_state, void* null)
{
    AppState* app = app_state;
    app->screen = SCREEN_MENU_EXIT;
}

static Menu* create_menu_start(SDL_Renderer* renderer, AppState* app)
{
    Menu* menu = menu_create(
        renderer,
        g_app_window_width / 2 - MENU_WIDTH / 4,
        g_app_window_height / 2 - MENU_HEIGHT / 8,
        MENU_WIDTH / 2,
        MENU_HEIGHT / 4
    );
    Function* func;
    func = function_create(menu_set_main, app, NULL);
    menu_add_button(renderer, menu, func, "!CHESS");

    return menu;
}

static Menu* create_menu_main(SDL_Renderer* renderer, AppState* app)
{
    Menu* menu = menu_create(
        renderer,
        g_app_window_width / 2 - MENU_WIDTH / 2,
        g_app_window_height / 2 - MENU_HEIGHT / 2,
        MENU_WIDTH,
        MENU_HEIGHT
    );
    Function* func;
    func = function_create(start_game, app, NULL);
    menu_add_button(renderer, menu, func, "START GAME");
    func = function_create(menu_set_options, app, NULL);
    menu_add_button(renderer, menu, func, "OPTIONS");
    func = function_create(menu_set_credits, app, NULL);
    menu_add_button(renderer, menu, func, "CREDITS");
    func = function_create(menu_set_instructions, app, NULL);
    menu_add_button(renderer, menu, func, "INSTRUCTIONS");
    func = function_create(menu_set_exit, app, NULL);
    menu_add_button(renderer, menu, func, "EXIT");
    return menu;
}

static Menu* create_menu_options(SDL_Renderer* renderer, AppState* app)
{
    Menu* menu = menu_create(
        renderer,
        g_app_window_width / 2 - MENU_WIDTH / 4,
        g_app_window_height / 2 - MENU_HEIGHT / 8,
        MENU_WIDTH / 2,
        MENU_HEIGHT / 4
    );
    Function* func;
    func = function_create(menu_set_main, app, NULL);
    menu_add_button(renderer, menu, func, "BACK");

    return menu;
}

static Menu* create_menu_credits(SDL_Renderer* renderer, AppState* app)
{
    Menu* menu = menu_create(
        renderer,
        g_app_window_width / 2 - MENU_WIDTH / 3,
        g_app_window_height / 2 - MENU_HEIGHT / 4,
        MENU_WIDTH / 1.5,
        MENU_HEIGHT / 2
    );
    Function* func;
    func = function_create(NULL, NULL, NULL);
    menu_add_button(renderer, menu, func, "Teresa Gamito");
    func = function_create(NULL, NULL, NULL);
    menu_add_button(renderer, menu, func, "Rodrigo Valente");
    func = function_create(menu_set_main, app, NULL);
    menu_add_button(renderer, menu, func, "BACK");

    return menu;
}

static Menu* create_menu_exit(SDL_Renderer* renderer, AppState* app)
{
    Menu* menu = menu_create(
        renderer,
        g_app_window_width / 2 - MENU_WIDTH / 4,
        g_app_window_height / 2 - MENU_HEIGHT / 4,
        MENU_WIDTH / 2,
        MENU_HEIGHT / 2
    );
    Function* func;
    func = function_create(app_quit, NULL, NULL);
    menu_add_button(renderer, menu, func, "CONFIRM");
    func = function_create(menu_set_main, app, NULL);
    menu_add_button(renderer, menu, func, "BACK");

    return menu;
}

static Menu* create_menu_instructions(SDL_Renderer* renderer, AppState* app)
{
    Menu* menu = menu_create(
        renderer,
        g_app_window_width / 2 - MENU_WIDTH,
        g_app_window_height / 2 - MENU_HEIGHT / 2,
        MENU_WIDTH * 2,
        MENU_HEIGHT
    );
    Function* func;
    func = function_create(NULL, NULL, NULL);
    menu_add_button(renderer, menu, func, "Left mouse button - Interact");
    func = function_create(NULL, NULL, NULL);
    menu_add_button(renderer, menu, func, "Tab - toggle between the Board and the Upgrade Tree");
    func = function_create(NULL, NULL, NULL);
    menu_add_button(renderer, menu, func, "Esc - pause");
    func = function_create(NULL, NULL, NULL);
    menu_add_button(renderer, menu, func, "Capture pieces to get points");
    func = function_create(NULL, NULL, NULL);
    menu_add_button(renderer, menu, func, "Spend points on upgrades for the game");
    func = function_create(NULL, NULL, NULL);
    menu_add_button(renderer, menu, func, "Each turn move a piece or purchase an upgrade");
    func = function_create(menu_set_main, app, NULL);
    menu_add_button(renderer, menu, func, "BACK");

    return menu;
}

void verify_app_state(const AppState* app)
{
    verify(app == NULL, "AppState does not exist");
}
