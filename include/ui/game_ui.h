#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "inputstate.h"
#include "helper_functions/global_variables.h"
#include "game/game.h"
#include "ui/board_ui.h"
#include "ui_elements/menu.h"

typedef struct GameUI GameUI;

typedef enum GameResult
{
    GAME_RESULT_CONTINUE,
    GAME_RESULT_WIN_WHITE,
    GAME_RESULT_WIN_BLACK,
    GAME_RESULT_EXIT
} GameResult;

GameUI* game_ui_create(Game* game, SDL_Renderer* renderer);
void game_ui_destroy(GameUI* ui);

void game_ui_render(SDL_Renderer* renderer, const GameUI* ui);
GameResult game_ui_update(InputState* input, GameUI* ui);

void game_ui_toggle_pause(GameUI* ui);

void verify_game_ui(const GameUI* ui);
