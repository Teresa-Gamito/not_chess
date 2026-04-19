#include "../include/appstate.h"

int g_app_window_width = APP_WINDOW_WIDTH;
int g_app_window_height = APP_WINDOW_HEIGHT;
float g_app_scale = 1;
int g_app_sprite_size_px = 16;

struct AppState
{
    SDL_Window* sdl_window;
    SDL_Renderer* sdl_renderer;

    InputState* input;

    GameState* gamestate;
};


// ========== create ==========
AppState* app_create()
{
    AppState* app = (AppState*)SDL_malloc(sizeof(AppState));
    verify(app == NULL, "AppState could not be created: malloc");
    
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

    gamestate_destroy(app->gamestate);

    SDL_free(app);
}



void game_start(AppState* app)
{
    verify(app == NULL, "AppState does not exist");

    app->gamestate = gamestate_create();
    gamestate_set_default(app->sdl_renderer, app->gamestate);
}



// ========== update ==========
void app_update(AppState* app)
{
    verify(app == NULL, "AppState does not exist");
    SDL_GetWindowSize(app->sdl_window, &g_app_window_width, &g_app_window_height);
    if (APP_WINDOW_WIDTH / APP_WINDOW_HEIGHT < g_app_window_width / g_app_window_height)
    {
        g_app_scale = (float) g_app_window_width / APP_WINDOW_WIDTH;
    }
    else
    {
        g_app_scale = (float) g_app_window_height / APP_WINDOW_HEIGHT;
    }

    gamestate_update(app_get_inputstate(app), app_get_gamestate(app));
}



// ========== render ==========
void app_render(AppState* app)
{
    verify(app == NULL, "AppState does not exist");

    SDL_Renderer* sdl_renderer = app_get_sdl_renderer(app);

    SDL_SetRenderDrawColor(sdl_renderer, 12, 23, 34, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(sdl_renderer);

    game_render(app->sdl_renderer, app->gamestate);

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
