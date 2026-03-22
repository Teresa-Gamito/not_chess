#include "../include/appstate.h"

struct AppState
{
    SDL_Window* sdl_window;
    SDL_Renderer* sdl_renderer;

    // change to array of pointers to windows
    Window* window;
    Textbox* textbox;
};


// ========== create ==========
AppState* appstate_create(
    SDL_Window* sdl_window, 
    SDL_Renderer* sdl_renderer, 
    Window* window,
    Textbox* textbox)
{
    AppState* appstate = (AppState*)SDL_malloc(sizeof(AppState));
    if (!appstate)
    {
        // TODO throw error
    }
    appstate->sdl_window = sdl_window;
    appstate->sdl_renderer = sdl_renderer;
    appstate->window = window;
    appstate->textbox = textbox;
    return appstate;
}

// ========== destroy ==========
void appstate_destroy(AppState* appstate)
{
    if (!appstate)
    {
        // TODO throw error
    }
    SDL_DestroyWindow(appstate->sdl_window);
    SDL_DestroyRenderer(appstate->sdl_renderer);
    window_destroy(appstate->window);
    textbox_destroy(appstate->textbox);
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


// change to array of pointers to windows
void appstate_set_window(AppState* appstate, Window* window)
{
    appstate->window = window;
}

void appstate_set_textbox(AppState* appstate, Textbox* textbox)
{
    appstate->textbox = textbox;
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

Window* appstate_get_window(const AppState* appstate)
{
    return appstate->window;
}

Textbox* appstate_get_textbox(const AppState* appstate)
{
    return appstate->textbox;
}
