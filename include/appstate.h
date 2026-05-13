// responsible for processing, updating and rendering all of the aspects of the app
#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>
#include "helper_functions/global_variables.h"
#include "inputstate.h"
#include "ui_elements/menu.h"
#include "game/ui/game_ui.h"

typedef struct AppState AppState;

// ========== create ==========
AppState* app_create();
void app_destroy(AppState* app);
void app_update(AppState* app);
void app_render(AppState* app);
void app_init(AppState* app);

InputState* app_get_inputstate(const AppState* app);

void verify_app_state(const AppState* app);
