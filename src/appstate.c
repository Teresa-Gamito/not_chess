#include "../include/appstate.h"

struct AppState
{
    SDL_Window* sdl_window;
    SDL_Renderer* sdl_renderer;

    InputState* input;
};


// ========== create ==========
AppState* app_create(SDL_Window* sdl_window, SDL_Renderer* sdl_renderer)
{
    AppState* app = (AppState*)SDL_malloc(sizeof(AppState));
    
    app->sdl_window = sdl_window;

    app->sdl_renderer = sdl_renderer;

    app->input = input_create();

    return app;
}

// ========== destroy ==========
void app_destroy(AppState* app)
{
    SDL_DestroyWindow(app->sdl_window);
    SDL_DestroyRenderer(app->sdl_renderer);
    input_destroy(app->input);
    SDL_free(app);
}



// update
void app_update(AppState* app)
{
    (void)app;
}



// ========== set ==========
void app_set_sdl_window(AppState* app, SDL_Window* sdl_window)
{
    app->sdl_window = sdl_window;
}
void app_set_sdl_renderer(AppState* app, SDL_Renderer* sdl_renderer)
{
    app->sdl_renderer = sdl_renderer;
}

// ========== get ==========
SDL_Window* app_get_sdl_window(const AppState* app)
{
    return app->sdl_window;
}
SDL_Renderer* app_get_sdl_renderer(const AppState* app)
{
    return app->sdl_renderer;
}

InputState* app_get_input(const AppState* app)
{
    return app->input;
}
