// holder for global variables
#pragma once

#include <stdbool.h>

#define APP_NAME "!Chess"
#define APP_WINDOW_WIDTH 1920
#define APP_WINDOW_HEIGHT 1080
#define APP_WINDOW_FLAGS SDL_WINDOW_RESIZABLE

#define FONT_PATH "assets/arialceb.ttf"
#define FONT_SIZE 32.0f

extern int g_app_window_width;
extern int g_app_window_height;
extern float g_app_scale;
extern int g_app_sprite_size_px;

extern bool can_purchace_multiple_times;
extern bool infinite_points;


