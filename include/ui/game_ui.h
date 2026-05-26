#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "inputstate.h"
#include "game/game.h"
#include "ui/board_ui.h"
#include "ui/tree_ui.h"
#include "ui/ui_elements/menu.h"
#include "ui/textures/ui_textures.h"
#include "game/properties/properties.h"
#include "helper_functions/global_variables.h"

#define screen_width g_app_window_width
#define screen_height g_app_window_height

#define UI_BUFFER screen_width / 128

#define UI_BUTTON_SIZE screen_width / 32

#define UI_PROPERTIES_WIDTH screen_width / 5
#define UI_PROPERTIES_HEIGHT screen_height / 2

#define UI_PLAYER_INFO_WIDTH screen_width / 5
#define UI_PLAYER_INFO_HEIGHT screen_height / 8

#define UI_RULES_WIDTH screen_width / 5
#define UI_RULES_HEIGHT screen_height / 2

#define UI_LOG_WIDTH screen_width / 5
#define UI_LOG_HEIGHT screen_height / 2

#define UI_PAUSE_WIDTH screen_width / 5
#define UI_PAUSE_HEIGHT screen_height / 2

#define UI_MINI_MAP_WIDTH screen_width / 5
#define UI_MINI_MAP_HEIGHT screen_width / 5

#define UI_UPGRADE_DESCRIPTION_WIDTH screen_width / 5
#define UI_UPGRADE_DESCRIPTION_HEIGHT screen_height / 2
#define UI_UPGRADE_DESCRIPTION_BUTTON_WIDTH UI_UPGRADE_DESCRIPTION_WIDTH - UI_BUFFER * 2
#define UI_UPGRADE_DESCRIPTION_BUTTON_HEIGHT UI_UPGRADE_DESCRIPTION_HEIGHT / 5

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

void game_ui_render(SDL_Renderer* renderer, GameUI* ui);
GameResult game_ui_update(InputState* input, GameUI* ui);

void verify_game_ui(const GameUI* ui);
