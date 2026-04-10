#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>
#include "inputstate.h"

typedef struct AppState AppState;


// ========== create ==========
AppState* app_create(SDL_Window* sdl_window, SDL_Renderer* sdl_renderer);


// ========== destroy ==========
void app_destroy(AppState* app);


// update
void app_update(AppState* app);


// set
void app_set_sdl_window(AppState* app, SDL_Window* window);
void app_set_sdl_renderer(AppState* app, SDL_Renderer* renderer);


// get
SDL_Window* app_get_sdl_window(const AppState* app);
SDL_Renderer* app_get_sdl_renderer(const AppState* app);
InputState* app_get_input(const AppState* app);
