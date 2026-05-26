#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>
#include "inputstate.h"
#include "ui/ui_elements/menu.h"
#include "ui/game_ui.h"
#include "helper_functions/global_variables.h"

#define MENU_WIDTH g_app_window_width / 4
#define MENU_HEIGHT g_app_window_height / 2

typedef struct AppState AppState;

// ========== create ==========
AppState* app_create();
void app_destroy(AppState* app);
void app_update(AppState* app);
void app_render(AppState* app);

InputState* app_get_inputstate(const AppState* app);

void verify_app_state(const AppState* app);
