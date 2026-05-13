#include "include/appstate.h"
#include "inputstate.h"
#include "ui_elements/menu.h"

typedef enum AppScreen
{
    APP_SCREEN_MAIN_MENU,
    APP_SCREEN_GAME,
} AppScreen;

struct AppState
{
    SDL_Window* sdl_window;
    SDL_Renderer* sdl_renderer;

    InputState* input;

    AppScreen screen;

    Menu* menu;

    Game* game;
    GameUI* game_ui;
};

static void app_end_game(void* app_state, void* null);
static void app_start_game(void* app_state, void* null);

static void app_menu_create(AppState* app);
static void app_menu_set_main_main(void* app_state, void* null);
static void app_menu_set_main_options(void* app_state, void* null);
static void app_menu_set_main_credits(void* app_state, void* null);
static void app_menu_set_main_quit(void* app_state, void* null);

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

    app->screen = 0;

    app->menu = NULL;
    app->game = NULL;
    app->game_ui = NULL;

    app->input = input_create();

    return app;
}

void app_init(AppState* app)
{
    verify_app_state(app);

    app_menu_create(app);
    app_menu_set_main_main(app, NULL);
    app_end_game(app, NULL);
}

void app_destroy(AppState* app)
{
    verify_app_state(app);

    SDL_DestroyWindow(app->sdl_window);
    SDL_DestroyRenderer(app->sdl_renderer);

    input_destroy(app->input);

    if (app->menu != NULL)
    {
        menu_destroy(app->menu);
    }
    app_end_game(app, NULL);

    TTF_Quit();

    SDL_free(app);
}


static void update_global_variables(AppState* app)
{
    SDL_GetWindowSize(app->sdl_window, &g_app_window_width, &g_app_window_height);
    if (APP_WINDOW_WIDTH / APP_WINDOW_HEIGHT < g_app_window_width / g_app_window_height)
    {
        g_app_scale = (float) g_app_window_width / APP_WINDOW_WIDTH;
    }
    else
    {
        g_app_scale = (float) g_app_window_height / APP_WINDOW_HEIGHT;
    }
    g_menu_font_size = FONT_SIZE_MENU * g_app_scale;
    g_game_font_size = FONT_SIZE * g_app_scale;
}

static void update_cheats(AppState* app)
{
    InputState* input = app->input;

    if (keyboard_get_pressed(input, SDL_SCANCODE_1))
    {
        infinite_points = !infinite_points;
    }
    if (keyboard_get_pressed(input, SDL_SCANCODE_2))
    {
        can_purchace_multiple_times = !can_purchace_multiple_times;
    }
}

static void update_screen(AppState* app)
{
    InputState* input = app->input;

    if (app->screen == APP_SCREEN_GAME)
    {
        game_ui_update(input, app->game_ui);
        return;
    }
    if (app->screen == APP_SCREEN_MAIN_MENU)
    {
        menu_update(input, app->menu);
        return;
    }
}

void app_update(AppState* app)
{
    verify_app_state(app);

    update_global_variables(app);
    update_cheats(app);
    update_screen(app);
}

static void render_screen(AppState* app)
{
    SDL_Renderer* renderer = app->sdl_renderer;

    if (app->screen == APP_SCREEN_MAIN_MENU)
    {
        menu_render(renderer, app->menu);
        return;
    }
    if (app->screen == APP_SCREEN_GAME)
    {
        game_ui_render(renderer, app->game_ui);
        return;
    }
}
void app_render(AppState* app)
{
    verify_app_state(app);

    SDL_Renderer* renderer = app->sdl_renderer;

    SDL_SetRenderDrawColor(renderer, 12, 23, 34, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    render_screen(app);

    SDL_RenderPresent(renderer);
}

InputState* app_get_inputstate(const AppState* app)
{
    verify_app_state(app);

    return app->input;
}

static void app_start_game(void* app_state, void* null)
{
    AppState* app = (AppState*)app_state;

    app_end_game(app, NULL);

    app->game = game_create();
    game_start(app->game);

    app->game_ui = game_ui_create(app->game, app->sdl_renderer);

    if (app->menu != NULL)
    {
        menu_destroy(app->menu);
        app->menu = NULL;
    }

    app->screen = APP_SCREEN_GAME;
}

static void app_end_game(void* app_state, void* null)
{
    AppState* app = (AppState*)app_state;
    if (app->game == NULL)
    {
        return;
    }
    game_destroy(app->game);
    app->game = NULL;

    if (app->game_ui == NULL)
    {
        return;
    }
    game_ui_destroy(app->game_ui);
    app->game_ui = NULL;

}

static void app_menu_create(AppState* app)
{
    app->menu = menu_create(
        app->sdl_renderer,
        (float)g_app_window_width / 4,
        (float)g_app_window_height / 4,
        (float)g_app_window_width / 2,
        (float)g_app_window_height / 2
    );
}

static void app_menu_set_main_main(void* app_state, void* null)
{
    AppState* app = (AppState*)app_state;
    SDL_Renderer* renderer = app->sdl_renderer;
    Menu* menu = app->menu;
    app->screen = APP_SCREEN_MAIN_MENU;

    menu_destroy_content(menu);
    Function* func;

    // START GAME
    func = function_create(app_start_game, app, NULL);
    menu_add_button(renderer, menu, func, "START GAME");
    // OPTIONS
    func = function_create(app_menu_set_main_options, app, NULL);
    menu_add_button(renderer, menu, func, "OPTIONS");
    // CREDITS
    func = function_create(app_menu_set_main_credits, app, NULL);
    menu_add_button(renderer, menu, func, "CREDITS");
    // QUIT
    func = function_create(app_menu_set_main_quit, app, NULL);
    menu_add_button(renderer, menu, func, "QUIT");
}

static void app_menu_set_main_options(void* app_state, void* null)
{
    AppState* app = (AppState*)app_state;
    SDL_Renderer* renderer = app->sdl_renderer;
    Menu* menu = app->menu;

    menu_destroy_content(menu);
    Function* func;

    // BACK
    func = function_create(app_menu_set_main_main, app, NULL);
    menu_add_button(renderer, menu, func, "BACK");
}

static void app_menu_set_main_credits(void* app_state, void* null)
{
    AppState* app = (AppState*)app_state;
    SDL_Renderer* renderer = app->sdl_renderer;
    Menu* menu = app->menu;

    menu_destroy_content(menu);
    Function* func;

    // BACK
    func = function_create(app_menu_set_main_main, app, NULL);
    menu_add_button(renderer, menu, func, "BACK");
}

static void app_menu_set_main_quit(void* app_state, void* null)
{
    AppState* app = (AppState*)app_state;
    SDL_Renderer* renderer = app->sdl_renderer;
    Menu* menu = app->menu;

    menu_destroy_content(menu);
    Function* func;

    // QUIT
    func = function_create(app_quit, NULL, NULL);
    menu_add_button(renderer, menu, func, "QUIT GAME");
    // BACK
    func = function_create(app_menu_set_main_main, app, NULL);
    menu_add_button(renderer, menu, func, "BACK");
}

void verify_app_state(const AppState* app)
{
    verify(app == NULL, "AppState does not exist");
}
