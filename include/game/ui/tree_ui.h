// responsible for rendering and processing input for the tree
#pragma once

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_render.h>
#include "helper_functions/error_handling.h"
#include "ui_elements/window.h"
#include "ui_elements/button.h"
#include "game/ui/tree_textures.h"
#include "game/ui/board_ui.h"
#include "game/tree/tree.h"
#include "helper_functions/color.h"
#include "helper_functions/global_variables.h"

typedef struct TreeUI TreeUI;


TreeUI* tree_ui_create(
    SDL_Renderer* renderer, 
    Tree* tree,
    BoardUI* board_ui,
    float x, 
    float y, 
    float width, 
    float height
);

void tree_ui_destroy(TreeUI* ui);

void tree_ui_update(TreeUI* ui);

Window* tree_ui_get_window(TreeUI* ui);

