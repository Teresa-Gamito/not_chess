#include "../include/appstate.h"

struct Appstate {
    SDL_Renderer *renderer; 
    SDL_Window *window; 
};

AppState *app_state_create();
void app_state_destroy(AppState *app_state);
