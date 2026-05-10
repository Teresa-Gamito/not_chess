// holder for global variables
#pragma once

#include <stdbool.h>

#define APP_NAME "!Chess"
#define APP_WINDOW_WIDTH 1920
#define APP_WINDOW_HEIGHT 1080
#define APP_WINDOW_FLAGS 0

#define SPRITE_SIZE_PX 16.0f

#define FONT_PATH "assets/fonts/arialceb.ttf"
#define FONT_SIZE 32.0f

// window properties
extern int g_app_window_width;
extern int g_app_window_height;
extern float g_app_scale;

// options
extern int g_app_volume;

// cheats
extern bool can_purchace_multiple_times;
extern bool infinite_points;

