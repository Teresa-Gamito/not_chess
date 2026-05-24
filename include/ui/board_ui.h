#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include "inputstate.h"
#include "game/board/board.h"
#include "game/board/board_elements/piece.h"
#include "game/board/board_elements/tile.h"
#include "game/upgrade_tree/upgrade.h"
#include "game/player/player.h"
#include "game/rules/rules.h"
#include "ui_elements/window.h"
#include "ui/textures/board_textures.h"
#include "data_structures/queue.h"
#include "helper_functions/position.h"
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

BoardUI* board_ui_create(SDL_Renderer* renderer, Board* board, Player* active_player, float x, float y, float width, float height);
void board_ui_destroy(BoardUI* ui);

BoardResult board_ui_update(InputState* input, BoardUI* ui);
void board_ui_render(SDL_Renderer* renderer, const BoardUI* ui);

void board_ui_set_active_player(BoardUI* ui, Player* player);

Board* board_ui_get_board(const BoardUI* ui);

void add_upgrade(BoardUI* ui, UpgradeType upgrade);

void verify_board_ui(const BoardUI* ui);

const char* board_selected_tile_get_properties(const BoardUI* ui);
