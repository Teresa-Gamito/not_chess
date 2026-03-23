#include "../include/app.h"

struct AppState
{
    SDL_Window* sdl_window;
    SDL_Renderer* sdl_renderer;
};


// ========== create ==========
AppState* appstate_create(SDL_Window* sdl_window, SDL_Renderer* sdl_renderer)
{
    AppState* appstate = (AppState*)SDL_malloc(sizeof(AppState));
    appstate->sdl_window = sdl_window;
    appstate->sdl_renderer = sdl_renderer;
    return appstate;
}

// ========== destroy ==========
void appstate_destroy(AppState* appstate)
{
    SDL_DestroyWindow(appstate->sdl_window);
    SDL_DestroyRenderer(appstate->sdl_renderer);
    SDL_free(appstate);
}

// ========== set ==========
void appstate_set_sdl_window(AppState* appstate, SDL_Window* sdl_window)
{
    appstate->sdl_window = sdl_window;
}
void appstate_set_sdl_renderer(AppState* appstate, SDL_Renderer* sdl_renderer)
{
    appstate->sdl_renderer = sdl_renderer;
}

// ========== get ==========
SDL_Window* appstate_get_sdl_window(const AppState* appstate)
{
    return appstate->sdl_window;
}
SDL_Renderer* appstate_get_sdl_renderer(const AppState* appstate)
{
    return appstate->sdl_renderer;
}
