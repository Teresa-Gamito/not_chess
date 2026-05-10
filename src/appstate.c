#include "include/appstate.h"
#include "game/gamestate.h"
#include "menu/menu.h"

typedef enum AppScreen
{
    APP_SCREEN_MAIN_MENU,
    APP_SCREEN_GAME,
    APP_SCREEN_PAUSE_MENU,
    APP_SCREEN_END_MENU
} AppScreen;

struct AppState
{
    SDL_Window* sdl_window;
    SDL_Renderer* sdl_renderer;

    InputState* input;

    AppScreen screen;

    Menu* menu;
    GameState* gamestate;
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
    app->gamestate = NULL;

    app->input = input_create();

    return app;
}

void app_init(AppState* app)
{
    app_set_screen(app, APP_SCREEN_MAIN_MENU);
}

static void app_end_game(AppState* app)
{
    if (app->gamestate == NULL)
    {
        return;
    }
    gamestate_destroy(app->gamestate);
    app->gamestate = NULL;
}

static void app_start_game(AppState* app)
{
    app_end_game(app);
    app->gamestate = gamestate_create();
    game_start(app->sdl_renderer, app->gamestate);
}

static void app_set_menu(AppState* app, MenuScreen menu)
{
    app->menu = menu_create(
        app->sdl_renderer,
        (float) g_app_window_width / 4,
        (float) g_app_window_height / 4,
        (float) g_app_window_width / 2,
        (float) g_app_window_height / 2,
        menu
    );
}
static void app_set_screen(AppState* app, AppScreen new_screen)
{
    SDL_Renderer* renderer = app->sdl_renderer;

    AppScreen old_screen = app->screen;
    switch (old_screen)
    {
        default:
        case APP_SCREEN_MAIN_MENU:
            app_set_menu(app, SCREEN_MENU_MAIN_MAIN);
            break;
        case APP_SCREEN_PAUSE_MENU:
            app_set_menu(app, SCREEN_MENU_PAUSE_MAIN);
            break;
        case APP_SCREEN_END_MENU:
            app_set_menu(app, SCREEN_MENU_END_MAIN);
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

    if (app->gamestate) gamestate_destroy(app->gamestate);
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

    GameState* game = app->gamestate;
    // game_update(input, game);

    menu_update(input, app->menu);
}



// ========== render ==========
void app_render(AppState* app)
{
    verify(app == NULL, "AppState does not exist");

    SDL_Renderer* sdl_renderer = app_get_sdl_renderer(app);

    SDL_SetRenderDrawColor(sdl_renderer, 12, 23, 34, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(sdl_renderer);

    // game_render(app->sdl_renderer, app->gamestate);
    menu_render(app->sdl_renderer, app->menu);

    SDL_RenderPresent(sdl_renderer);
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

GameState* app_get_gamestate(const AppState* app)
{
    verify(app == NULL, "AppState does not exist");

    return app->gamestate;
}
