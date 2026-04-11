#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>
#include "inputstate.h"
#include "game/gamestate.h"

typedef struct AppState AppState;


// ========== create ==========
AppState* app_create(SDL_Window* sdl_window, SDL_Renderer* sdl_renderer);

// ========== destroy ==========
void app_destroy(AppState* app);

// ========== update ==========
void app_update(AppState* app);

// ========== render ==========
void app_render(AppState* app);

// ========== get ==========
SDL_Window* app_get_sdl_window(const AppState* app);
SDL_Renderer* app_get_sdl_renderer(const AppState* app);
InputState* app_get_inputstate(const AppState* app);
GameState* app_get_gamestate(const AppState* app);
