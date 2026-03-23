#include "../include/main.h"

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) 
{
    (void)argc;
    (void)argv;

    // Inits
    SDL_Init(SDL_INIT_VIDEO);

    // create window
    SDL_Window* sdl_window = SDL_CreateWindow("test", 1000, 1000, 0);
    // create renderer
    SDL_Renderer* sdl_renderer = SDL_CreateRenderer(sdl_window, NULL);

    // create appstate
    AppState* app = appstate_create(sdl_window, sdl_renderer);

    // associate appstate
    *appstate = app;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) 
{
    (void) appstate;
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) 
{
    AppState* app = (AppState*)appstate;
    SDL_Renderer* sdl_renderer = appstate_get_sdl_renderer(app);

    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(sdl_renderer);

    SDL_RenderPresent(sdl_renderer);
    return SDL_APP_CONTINUE; 
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) 
{
    (void)result;

    AppState* app = (AppState*)appstate;
    appstate_destroy(app);

    SDL_Quit();
}

