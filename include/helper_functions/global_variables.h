// holder for global variables
#pragma once

#include <stdbool.h>

// app
#define APP_NAME "!Chess"
#define APP_WINDOW_WIDTH 1920
#define APP_WINDOW_HEIGHT 1080
#define APP_WINDOW_FLAGS 0

// sprites
#define SPRITE_SIZE_PX 16.0f

// fonts
#define FONT_PATH "assets/fonts/arialceb.ttf"
#define FONT_SIZE 32.0f

#define FONT_PATH_MENU "assets/fonts/arialceb.ttf"
#define FONT_SIZE_MENU 32.0f

// game
#define GAME_STARTING_COL_NUM 8
#define GAME_STARTING_ROW_NUM 8

// window properties
extern float g_app_window_width;
extern float g_app_window_height;

extern float g_app_scale;

extern float g_menu_font_size;
extern float g_game_font_size;

// options
extern int g_app_volume;

// cheats
extern bool debug_can_purchace_multiple_times;
extern bool debug_infinite_points;

