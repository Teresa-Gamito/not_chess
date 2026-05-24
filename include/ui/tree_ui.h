#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "inputstate.h"
#include "game/upgrade_tree/tree.h"
#include "ui/ui_elements/window.h"
#include "helper_functions/color.h"
#include "ui/textures/tree_textures.h"
#include "helper_functions/helper_functions.h"
#include "helper_functions/global_variables.h"

typedef struct TreeUI TreeUI;

TreeUI* tree_ui_create(SDL_Renderer* renderer, Tree* tree, float x, float y, float width, float height);
void tree_ui_destroy(TreeUI* ui);

void tree_ui_render(SDL_Renderer* renderer, const TreeUI* ui);
void tree_ui_update(InputState* input, TreeUI* ui);
