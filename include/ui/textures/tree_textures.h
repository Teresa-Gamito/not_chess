#pragma once

#define TEXTURE_DEFAULT_SIZE_PX 16

#define PATH_TEXTURE_NODE_ADD_PAWN              "assets/sprites/node/node_add_pawn.png"
#define PATH_TEXTURE_NODE_EXPAND_BOARD          "assets/sprites/node/node_expand_board.png"
#define PATH_TEXTURE_NODE_ADD_LANCE             "assets/sprites/node/node_add_lance.png"
#define PATH_TEXTURE_NODE_SACRIFICE             "assets/sprites/node/node_sacrifice.png"

#define PATH_TEXTURE_TREE_BUTTON_IDLE           "assets/sprites/tile/tile_selected.png"
#define PATH_TEXTURE_TREE_BUTTON_HOVERED        "assets/sprites/tile/tile_valid.png"
#define PATH_TEXTURE_TREE_BUTTON_PRESSED        "assets/sprites/tile/tile_capture.png"

#define PATH_TEXTURE_WINDOW_TREE_BACKGROUND     "assets/sprites/tile/tile_black.png"

typedef enum TreeTextures
{
    TEXTURE_NODE_ADD_PAWN,
    TEXTURE_NODE_EXPAND_BOARD,
    TEXTURE_NODE_ADD_LANCE,
    TEXTURE_NODE_SACRIFICE,

    TEXTURE_TREE_BUTTON_IDLE,
    TEXTURE_TREE_BUTTON_HOVERED,
    TEXTURE_TREE_BUTTON_PRESSED,

    TREE_TEXTURES_COUNT
} TreeTextures;

static const char* tree_textures[] = 
{
    PATH_TEXTURE_NODE_ADD_PAWN,
    PATH_TEXTURE_NODE_EXPAND_BOARD,
    PATH_TEXTURE_NODE_ADD_LANCE,
    PATH_TEXTURE_NODE_SACRIFICE,

    PATH_TEXTURE_TREE_BUTTON_IDLE,
    PATH_TEXTURE_TREE_BUTTON_HOVERED,
    PATH_TEXTURE_TREE_BUTTON_PRESSED,
};
