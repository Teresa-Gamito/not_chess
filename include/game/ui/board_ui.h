// responsible for rendering and processing input for the board
#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include "helper_functions/error_handling.h"
#include "helper_functions/helper_functions.h"
#include "game/ui/board_textures.h"
#include "helper_functions/vector.h"
#include "ui_elements/window.h"
#include "ui_elements/button.h"
#include "game/board/board.h"
#include "game/board/player.h"
#include "game/board/piece.h"
#include "game/board/task.h"
#include "game/log.h"

typedef struct BoardUI BoardUI;

BoardUI* board_ui_create(
    SDL_Renderer* renderer,
    Board* board,
    float x,
    float y,
    float width,
    float height
);
void board_ui_destroy(BoardUI* ui);
void board_ui_update(BoardUI* ui);

void select_tile(void* board_ui, void* tile);
void deselect_tile(void* board_ui, void* x);

void board_ui_add_task(BoardUI* ui, Task task);

Window* board_ui_get_window(const BoardUI* ui);
Board* board_ui_get_board(const BoardUI* ui);

void verify_board_ui(const BoardUI* ui);
