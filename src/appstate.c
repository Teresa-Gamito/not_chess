#include "include/appstate.h"
#include "helper_functions/global_variables.h"
#include "menu/menu.h"

struct AppState
{
    SDL_Window* sdl_window;
    SDL_Renderer* sdl_renderer;

    InputState* input;

    Menu* menu;

    GameState* gamestate;
};


// ========== create ==========
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

    app->menu = NULL;
    app->gamestate = NULL;

    app->input = input_create();

    return app;
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



void app_game_start(AppState* app)
{
    verify(app == NULL, "AppState does not exist");

    app->menu = menu_create(
        app->sdl_renderer,
        0,
        0,
        g_app_window_width,
        g_app_window_height,
        SCREEN_MENU_MAIN_MAIN
    );
    // app->gamestate = gamestate_create();
    // game_start(app->sdl_renderer, app->gamestate);
}



// ========== update ==========
void app_update(AppState* app)
{
    verify(app == NULL, "AppState does not exist");

    InputState* input = app->input;
    GameState* game = app->gamestate;

    SDL_GetWindowSize(app->sdl_window, &g_app_window_width, &g_app_window_height);
    if (APP_WINDOW_WIDTH / APP_WINDOW_HEIGHT < g_app_window_width / g_app_window_height)
    {
        g_app_scale = (float) g_app_window_width / APP_WINDOW_WIDTH;
    }
    else
    {
        g_app_scale = (float) g_app_window_height / APP_WINDOW_HEIGHT;
    }

    if (keyboard_get_pressed(input, SDL_SCANCODE_1))
    {
        infinite_points = !infinite_points;
    }
    if (keyboard_get_pressed(input, SDL_SCANCODE_2))
    {
        can_purchace_multiple_times = !can_purchace_multiple_times;
    }
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
