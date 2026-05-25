#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include "game/game.h"
#include "inputstate.h"
#include "game/game.h"
#include "game/upgrade.h"
#include "ui_elements/window.h"
#include "ui/textures/board_textures.h"
#include "helper_functions/error_handling.h"
#include "helper_functions/helper_functions.h"

typedef enum BoardResult
{
    BOARD_RESULT_CONTINUE,
    BOARD_RESULT_ADVANCE_TURN,
    BOARD_RESULT_WIN_WHITE,
    BOARD_RESULT_WIN_BLACK
} BoardResult;

typedef struct BoardUI BoardUI;

BoardUI* board_ui_create(SDL_Renderer* renderer, Game* game, float x, float y, float width, float height);
void board_ui_destroy(BoardUI* ui);

void board_ui_update(InputState* input, BoardUI* ui);
void board_ui_render(SDL_Renderer* renderer, const BoardUI* ui);

void board_ui_update_objects(BoardUI* ui);

void board_ui_add_upgrade(BoardUI* ui, UpgradeType upgrade);

void board_ui_set_scale(BoardUI* ui);
SDL_Texture* piece_get_texture(BoardUI* ui, Pos pos);
SDL_Texture* tile_get_texture(BoardUI* ui, Pos pos);
bool board_ui_has_selected_pos(const BoardUI* ui);
Pos board_ui_get_selected_pos(const BoardUI* ui);
Window* board_ui_get_window(const BoardUI* ui);

void select_pos(BoardUI* ui, Pos pos);
void deselect_pos(BoardUI* ui);


void verify_board_ui(const BoardUI* ui);

const char* board_selected_tile_get_properties(const BoardUI* ui);
