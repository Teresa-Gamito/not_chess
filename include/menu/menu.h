#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "helper_functions/helper_functions.h"
#include "helper_functions/function.h"
#include "helper_functions/vector.h"
#include "helper_functions/color.h"
#include "ui_elements/window.h"
#include "ui_elements/button.h"
#include "ui_elements/textbox.h"
#include "menu/menu_textures.h"
#include "inputstate.h"

#define FONT_PATH_MENU "assets/fonts/arialceb.ttf"
#define FONT_SIZE_MENU 32.0f

typedef struct Menu Menu;

typedef enum Screen
{
    SCREEN_MENU_MAIN_MAIN,
    SCREEN_MENU_MAIN_OPTIONS,
    SCREEN_MENU_MAIN_CREDITS,
    SCREEN_MENU_MAIN_QUIT,

    SCREEN_MENU_PAUSE_MAIN,
    SCREEN_MENU_PAUSE_OPTIONS,
    SCREEN_MENU_PAUSE_QUIT,
} Screen;

Menu* menu_create(SDL_Renderer* renderer, float x, float y, float width, float height, Screen starting_screen);
void menu_destroy(Menu* menu);
void menu_render(SDL_Renderer* renderer, Menu* menu);
void menu_update(InputState* input, Menu* menu);
