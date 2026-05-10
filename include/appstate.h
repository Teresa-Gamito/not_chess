// responsible for processing, updating and rendering all of the aspects of the app
#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>
#include "helper_functions/global_variables.h"
#include "game/gamestate.h"
#include "inputstate.h"
#include "menu/menu.h"

typedef struct AppState AppState;

// ========== create ==========
AppState* app_create();
void app_destroy(AppState* app);
void app_update(AppState* app);
void app_render(AppState* app);

SDL_Window* app_get_sdl_window(const AppState* app);
SDL_Renderer* app_get_sdl_renderer(const AppState* app);
InputState* app_get_inputstate(const AppState* app);
GameState* app_get_gamestate(const AppState* app);
