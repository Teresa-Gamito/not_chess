#include "../include/appstate.h"

struct Appstate {
    // SDL_Renderer *renderer; 
    // SDL_Window *window; 
};

AppState *app_state_create()
{
    AppState *app_state = malloc(sizeof(AppState));
    if (!app_state)
    {
        // throw error
    }
    return app_state;
}

void app_state_destroy(AppState *app_state)
{

}
