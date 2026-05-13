#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "inputstate.h"
#include "helper_functions/global_variables.h"
#include "game/game.h"
#include "game/ui/board_ui.h"
#include "game/ui/tree_ui.h"
#include "ui_elements/menu.h"

typedef struct GameUI GameUI;

GameUI* game_ui_create(Game* game, SDL_Renderer* renderer);
void game_ui_destroy(GameUI* ui);

void game_ui_render(SDL_Renderer* renderer, const GameUI* ui);
void game_ui_update(InputState* input, GameUI* ui);

void game_ui_toggle_pause(GameUI* ui);

void verify_game_ui(const GameUI* ui);
