#pragma once

#define TEXTURE_DEFAULT_SIZE_PX 16

// BUTTONS
#define PATH_TEXTURE_BUTTON_RULES                   "assets/sprites/tile/tile_valid.png"
#define PATH_TEXTURE_BUTTON_LOG                     "assets/sprites/tile/tile_capture.png"
#define PATH_TEXTURE_BUTTON_HELP                    "assets/sprites/tile/tile_selected.png"
#define PATH_TEXTURE_BUTTON_PAUSE                   "assets/sprites/tile/tile_white.png"

#define PATH_TEXTURE_WINDOW_PROPERTIES              "assets/sprites/tile/tile_black.png"
#define PATH_TEXTURE_WINDOW_PLAYER_INFO             "assets/sprites/tile/tile_black.png"
#define PATH_TEXTURE_WINDOW_RULES             "assets/sprites/tile/tile_black.png"

typedef enum UIButtonsTextures
{
    TEXTURE_BUTTON_RULES,
    TEXTURE_BUTTON_LOG,
    TEXTURE_BUTTON_HELP,
    TEXTURE_BUTTON_PAUSE,

    UI_BUTTONS_TEXTURE_COUNT
} UIButtonsTextures;

static const char* ui_buttons_textures[] =
{
    PATH_TEXTURE_BUTTON_RULES,
    PATH_TEXTURE_BUTTON_LOG,
    PATH_TEXTURE_BUTTON_HELP,
    PATH_TEXTURE_BUTTON_PAUSE
};
