#include "../include/main.h"
#include <SDL3_ttf/SDL_ttf.h>

Object* object = NULL;
TTF_Font* font = NULL;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) 
{
    (void)argc;
    (void)argv;

    // Inits
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    // create window
    SDL_Window* sdl_window = SDL_CreateWindow("test", 1000, 1000, 0);
    
    // create renderer
    SDL_Renderer* sdl_renderer = SDL_CreateRenderer(sdl_window, NULL);

    // create font
    TTF_Font* font = TTF_OpenFont("./arialceb.ttf", 30.0f);

    // create color
    SDL_Color* color = (SDL_Color*)SDL_malloc(sizeof(SDL_Color));
    color->r = 255;
    color->g = 255;
    color->b = 255;
    color->a = 100;

    // create appstate
    AppState* app;
    app = appstate_create(
        sdl_window,
        sdl_renderer,
        window_create(
            0, 
            0, 
            200, 
            200, 
            NONE
        ),
        textbox_create(
            sdl_renderer,
            500,
            500,
            200,
            "THIS IS A TEST OwO OwO OwO OwO OwO OwO OwO OwO OwO OwO",
            font,
            color
        )
    );
    object = object_create(sdl_renderer, 10, 10, "test.png");

    // associate appstate
    *appstate = app;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) 
{
    // AppState* app = (AppState*)appstate;
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
    Window* window = appstate_get_window(app);
    Textbox* text = appstate_get_textbox(app);

    SDL_SetRenderDrawColor(sdl_renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(sdl_renderer);


    
    window_draw(sdl_renderer, window);

    SDL_RenderTexture(
        sdl_renderer, 
        textbox_get_texture(text), 
        NULL,
        textbox_get_frect(text)       
    );

    SDL_RenderTexture(
        sdl_renderer,
        object_get_texture(object),
        NULL,
        object_get_frect(object)
    );

    SDL_RenderPresent(sdl_renderer);
    return SDL_APP_CONTINUE; 
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) 
{
    SDL_Log("ERROR: %s", SDL_GetError()); (void)result;
    AppState* app = (AppState*)appstate;
    appstate_destroy(app);
    object_destroy(object);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}

