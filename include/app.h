#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct AppState AppState;

// ========== create ==========
AppState* appstate_create(SDL_Window* sdl_window, SDL_Renderer* sdl_renderer);

// ========== destroy ==========
void appstate_destroy(AppState* appstate);


// set
void appstate_set_sdl_window(AppState* appstate, SDL_Window* window);
void appstate_set_sdl_renderer(AppState* appstate, SDL_Renderer* renderer);


// get
SDL_Window* appstate_get_sdl_window(const AppState* appstate);
SDL_Renderer* appstate_get_sdl_renderer(const AppState* appstate);
