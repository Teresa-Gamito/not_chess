#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "main.h"

typedef struct AppState {
    SDL_Renderer *sld_renderer;
    SDL_Window *sdl_window;

    Window *windows; // window array
} AppState;


SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) 
{
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) 
{
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) 
{
    return SDL_APP_CONTINUE; 
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) 
{
    
}
