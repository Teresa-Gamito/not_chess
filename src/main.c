#include "include/main.h"

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) 
{
    (void)argc;
    (void)argv;

    SDL_Init(SDL_INIT_VIDEO);

    AppState* app = app_create();
    *appstate = app;

    app_game_start(app);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) 
{
    AppState* app = (AppState*)appstate;
    (void)app;

    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }

    input_update(app_get_inputstate(app), event);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) 
{
    AppState* app = (AppState*)appstate;

    // update logic
    app_update(app);

    // render
    app_render(app);

    // reset input
    input_begin_frame(app_get_inputstate(app));

    SDL_Delay(1);
    return SDL_APP_CONTINUE; 
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) 
{
    (void)result;

    AppState* app = (AppState*)appstate;
    app_destroy(app);

    SDL_Quit();
}
