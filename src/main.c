#include "../header/main.h"

typedef struct BoardState {
} BoardState;

typedef struct MenuState {
} MenuState;

typedef struct ScreenState {
} ScreenState;

typedef struct AppState {
    SDL_Renderer* sld_renderer;
    SDL_Window* sdl_window;

    // sub-sections
    BoardState board_state;
    MenuState menu_state;
    ScreenState screen_state;
} AppState;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) 
{
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) 
{
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) 
{
    return SDL_APP_CONTINUE; 
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) 
{
    
}
