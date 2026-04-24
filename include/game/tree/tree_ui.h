#pragma once

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_render.h>
#include "helper_functions/error_handling.h"
#include "ui_elements/window.h"
#include "ui_elements/button.h"
#include "game/tree/tree_textures.h"
#include "game/board/board_ui.h"
#include "game/tree/tree.h"

typedef struct TreeUI TreeUI;


TreeUI* tree_ui_create(
    SDL_Renderer* renderer,
    Tree* tree,
    Board* board,
    float x, 
    float y, 
    float width, 
    float height
);
void tree_ui_destroy(TreeUI* ui);

Window* tree_ui_get_window(TreeUI* ui);

void tree_ui_update(TreeUI* ui);
