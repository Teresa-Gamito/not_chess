#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "inputstate.h"
#include "game/game.h"
#include "ui/board_ui.h"
#include "ui/tree_ui.h"
#include "ui/textures/ui_textures.h"
#include "helper_functions/global_variables.h"

#define screen_width g_app_window_width
#define screen_height g_app_window_height

#define UI_BUFFER screen_width / 128

#define UI_BUTTON_SIZE screen_width / 32
#define UI_BUTTON_BUFFER UI_BUTTON_SIZE / 8

#define UI_PROPERTIES_WIDTH screen_width / 5
#define UI_PROPERTIES_HEIGHT screen_height / 2

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

void verify_game_ui(const GameUI* ui);
