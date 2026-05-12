#include "include/appstate.h"
#include "game/game.h"

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

static void app_set_screen(AppState* app, AppScreen new_screen);

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

    app->screen = APP_SCREEN_MAIN_MENU;

    app->menu = NULL;
    app->game = NULL;
    app->game_ui = NULL;

    app->input = input_create();

    return app;
}

void app_init(AppState* app)
{
    app_set_screen(app, APP_SCREEN_GAME);
}


static void app_set_screen(AppState* app, AppScreen new_screen)
{
    switch (new_screen)
    {
        default:
        case APP_SCREEN_MAIN_MENU:
            // TODO: 
            break;
        case APP_SCREEN_GAME:
            app_start_game(app);
            break;
    }
    app->screen = new_screen;
}




// ========== destroy ==========
void app_destroy(AppState* app)
{
    verify(app == NULL, "AppState does not exist");

    SDL_DestroyWindow(app->sdl_window);
    SDL_DestroyRenderer(app->sdl_renderer);

    input_destroy(app->input);

    app_end_game(app);
    if (app->menu) menu_destroy(app->menu);

    TTF_Quit();

    SDL_free(app);
}



// ========== update ==========
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
static void update_cheats(InputState* input)
{
    if (keyboard_get_pressed(input, SDL_SCANCODE_1))
    {
        infinite_points = !infinite_points;
    }
    if (keyboard_get_pressed(input, SDL_SCANCODE_2))
    {
        can_purchace_multiple_times = !can_purchace_multiple_times;
    }
}
void app_update(AppState* app)
{
    verify(app == NULL, "AppState does not exist");

    InputState* input = app->input;

    update_global_variables(app);
    update_cheats(input);

    if (app->screen == APP_SCREEN_GAME)
    {
        game_ui_update(input, app->game_ui);
    }
    else
    {
        menu_update(input, app->menu);
    }
}



// ========== render ==========
void app_render(AppState* app)
{
    verify(app == NULL, "AppState does not exist");

    SDL_Renderer* renderer = app_get_sdl_renderer(app);

    SDL_SetRenderDrawColor(renderer, 12, 23, 34, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    if (app->screen == APP_SCREEN_GAME)
    {
        game_ui_render(renderer, app->game_ui);
    }
    else
    {
        menu_render(renderer, app->menu);
    }

    SDL_RenderPresent(renderer);
}



// ========== get ==========
SDL_Window* app_get_sdl_window(const AppState* app)
{
    verify(app == NULL, "AppState does not exist");

    return app->sdl_window;
}
SDL_Renderer* app_get_sdl_renderer(const AppState* app)
{
    verify(app == NULL, "AppState does not exist");

    return app->sdl_renderer;
}

InputState* app_get_inputstate(const AppState* app)
{
    verify(app == NULL, "AppState does not exist");

    return app->input;
}

static void menu_set_main_main(void* app_state, void* null);
static void menu_set_main_options(void* app_state, void* null);
static void menu_set_main_credits(void* app_state, void* null);
static void menu_set_main_exit(void* app_state, void* null);
static void app_end_game(void* app_state, void* null);
static void app_start_game(void* app_state, void* null);

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

static void app_start_game(void* app_state, void* null)
{
    AppState* app = (AppState*)app_state;
    app_end_game(app);
    app->game = game_create();
    game_start(app->game);
    app->game_ui = game_ui_create(app->game, app->sdl_renderer);
}
static void menu_set_main_main(void* app_state, void* null)
{
    AppState* app = (AppState*)app_state;
    SDL_Renderer* renderer = app->sdl_renderer;
    Menu* menu = app->menu;

    menu_destroy_content(menu);
    Function* func;

    func = function_create();
    menu_add_button(renderer, menu, func, "START GAME");
    func = function_create();
    menu_add_button(renderer, menu, func, "OPTIONS");
    func = function_create();
    menu_add_button(renderer, menu, func, "CREDITS");
    func = function_create();
    menu_add_button(renderer, menu, func, "QUIT");
}

static void menu_set_main_options(void* app_state, void* null)
{
    AppState* app = (AppState*)app_state;
    SDL_Renderer* renderer = app->sdl_renderer;
    Menu* menu = app->menu;

    menu_destroy_content(menu);
    Function* func;

    func = function_create(menu_set_main_main, renderer, menu);
    menu_add_button(renderer, menu, func, "BACK");
}

static void menu_set_main_credits(void* app_state, void* null)
{
    AppState* app = (AppState*)app_state;
    SDL_Renderer* renderer = app->sdl_renderer;
    Menu* menu = app->menu;

    menu_destroy_content(menu);
    Function* func;

    func = function_create(menu_set_main_main, renderer, menu);
    menu_add_button(renderer, menu, func, "BACK");
}

static void menu_set_main_exit(void* app_state, void* null)
{
    AppState* app = (AppState*)app_state;
    SDL_Renderer* renderer = app->sdl_renderer;
    Menu* menu = app->menu;

    menu_destroy_content(menu);
    Function* func;

    func = function_create(app_quit, NULL, NULL);
    menu_add_button(renderer, menu, func, "QUIT");
    func = function_create(menu_set_main_main, renderer, menu);
    menu_add_button(renderer, menu, func, "BACK");
}
