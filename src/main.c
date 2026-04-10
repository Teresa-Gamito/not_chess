#include "../include/main.h"

// testing
SDL_Texture* window_background = NULL;
Window* window = NULL;

SDL_Texture* black_pawn_texture = NULL;
Object* object = NULL;

Textbox* textbox = NULL;

SDL_Texture* button_texture_idle = NULL;
SDL_Texture* button_texture_pressed = NULL;
SDL_Texture* button_texture_hovered = NULL;
Button* button = NULL;

void test_print()
{
    SDL_Log("OwO");
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) 
{
    (void)argc;
    (void)argv;

    SDL_Init(SDL_INIT_VIDEO);



    SDL_Window* sdl_window = SDL_CreateWindow("test", 1000, 1000, 0);

    SDL_Renderer* sdl_renderer = SDL_CreateRenderer(sdl_window, NULL);
    SDL_SetDefaultTextureScaleMode(sdl_renderer, SDL_SCALEMODE_PIXELART);


    AppState* app = app_create(sdl_window, sdl_renderer);
    *appstate = app;


    // ========== testing area ==========
    window_background = SDL_CreateTextureFromPNG(sdl_renderer, "assets/sprites/tile_white.png");
    window = window_create(250, 250, 500, 500, window_background, NONE);

    black_pawn_texture = SDL_CreateTextureFromPNG(sdl_renderer, "assets/sprites/black_pawn.png");
    object = object_create(64, 0, black_pawn_texture);
    object_set_size(object, 64, 64);
    window_add_object(window, object);

    button_texture_idle = SDL_CreateTextureFromPNG(sdl_renderer, "assets/sprites/black_pawn.png");
    button_texture_hovered = SDL_CreateTextureFromPNG(sdl_renderer, "assets/sprites/black_queen.png");
    button_texture_pressed = SDL_CreateTextureFromPNG(sdl_renderer, "assets/sprites/black_king.png");
    button = button_create(0, 0, test_print, button_texture_idle, button_texture_pressed, button_texture_hovered);
    button_set_size(button, 64, 64);
    window_add_button(window, button);


    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) 
{
    AppState* app = (AppState*)appstate;

    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }

    input_update(app_get_input(app), event);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) 
{
    AppState* app = (AppState*)appstate;
    SDL_Renderer* sdl_renderer = app_get_sdl_renderer(app);


    // update logic
    app_update(app);
    window_update(app_get_input(app), window);


    // render
    SDL_SetRenderDrawColor(sdl_renderer, 12, 23, 34, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(sdl_renderer);
    window_render(sdl_renderer, window);
    SDL_RenderPresent(sdl_renderer);


    // reset input
    input_begin_frame(app_get_input(app));

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

