#pragma once

#define TEXTURE_DEFAULT_SIZE_PX 16

#define PATH_TEXTURE_NODE_ADD_PAWN              "assets/sprites/white_pawn.png"

#define PATH_TEXTURE_TREE_BUTTON_IDLE           "assets/sprites/tile_selected.png"
#define PATH_TEXTURE_TREE_BUTTON_HOVERED        "assets/sprites/tile_valid.png"
#define PATH_TEXTURE_TREE_BUTTON_PRESSED        "assets/sprites/tile_capture.png"

#define PATH_TEXTURE_WINDOW_TREE_BACKGROUND     "assets/sprites/tile_black.png"

typedef enum TreeTextures
{
    TEXTURE_NODE_ADD_PAWN,

    TEXTURE_TREE_BUTTON_IDLE,
    TEXTURE_TREE_BUTTON_HOVERED,
    TEXTURE_TREE_BUTTON_PRESSED,

    TREE_TEXTURES_COUNT
} TreeTextures;

static const char* tree_textures[] = 
{
    PATH_TEXTURE_NODE_ADD_PAWN,

    PATH_TEXTURE_TREE_BUTTON_IDLE,
    PATH_TEXTURE_TREE_BUTTON_HOVERED,
    PATH_TEXTURE_TREE_BUTTON_PRESSED,
};
