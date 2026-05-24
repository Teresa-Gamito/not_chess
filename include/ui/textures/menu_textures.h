#pragma once

#define PATH_TEXTURE_MENU_BACKGROUND "assets/sprites/tile/tile_black.png"

#define PATH_TEXTURE_MENU_BUTTON "assets/sprites/tile/tile_white.png"

typedef enum MenuTextures
{
    TEXTURE_MENU_BUTTON,

    MENU_TEXTURE_COUNT
} MenuTextures;

static const char* menu_textures[] =
{
    PATH_TEXTURE_MENU_BUTTON
};
