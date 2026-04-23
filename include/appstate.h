#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>
#include "inputstate.h"
#include "include/game/gamestate.h"

#define APP_NAME "!Chess"
#define APP_WINDOW_WIDTH 1000
#define APP_WINDOW_HEIGHT 1000
#define APP_WINDOW_FLAGS SDL_WINDOW_RESIZABLE

typedef struct AppState AppState;

extern int g_app_window_width;
extern int g_app_window_height;
extern float g_app_scale;
extern int g_app_sprite_size_px;

// ========== create ==========
AppState* app_create();
void app_game_start(AppState* app);

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
