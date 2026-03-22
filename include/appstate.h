#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>
#include "textbox.h"
#include "window.h"

typedef struct AppState AppState;

// create / destroy
AppState* appstate_create(
    SDL_Window* sdl_window, 
    SDL_Renderer* sdl_renderer,
    Window* window,
    Textbox* textbox
);
void appstate_destroy(AppState* appstate);


// set
void appstate_set_sdl_window(AppState* appstate, SDL_Window* window);
void appstate_set_sdl_renderer(AppState* appstate, SDL_Renderer* renderer);

void appstate_set_window(AppState* appstate, Window* window);
void appstate_set_textbox(AppState* appstate, Textbox* textbox);


// get
SDL_Window* appstate_get_sdl_window(const AppState* appstate);
SDL_Renderer* appstate_get_sdl_renderer(const AppState* appstate);

Window* appstate_get_window(const AppState* appstate);
Textbox* appstate_get_textbox(const AppState* appstate);
