#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "helper_functions/helper_functions.h"
#include "helper_functions/function.h"
#include "data_structures/vector.h"
#include "helper_functions/color.h"
#include "ui/ui_elements/window.h"
#include "ui/ui_elements/button.h"
#include "ui/ui_elements/textbox.h"
#include "ui/textures/menu_textures.h"
#include "inputstate.h"


typedef struct Menu Menu;

Menu* menu_create(SDL_Renderer* renderer, float x, float y, float width, float height);
void menu_destroy(Menu* menu);
void menu_destroy_content(Menu* menu);
void menu_render(SDL_Renderer* renderer, Menu* menu);
void menu_update(InputState* input, Menu* menu);

void menu_add_button(SDL_Renderer* renderer, Menu* menu, Function* function, const char* text);

void verify_menu(const Menu* menu);
