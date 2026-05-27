#pragma once

#define TEXTURE_DEFAULT_SIZE_PX 16

#define PATH_TEXTURE_LINE_UD                    "assets/sprites/arrows/line_ud.png"
#define PATH_TEXTURE_LINE_LR                    "assets/sprites/arrows/line_lr.png"
#define PATH_TEXTURE_LINE_DL                    "assets/sprites/arrows/line_dl.png"
#define PATH_TEXTURE_LINE_DR                    "assets/sprites/arrows/line_dr.png"
#define PATH_TEXTURE_LINE_UL                    "assets/sprites/arrows/line_ul.png"
#define PATH_TEXTURE_LINE_UR                    "assets/sprites/arrows/line_ur.png"
#define PATH_TEXTURE_LINE_UDLR                  "assets/sprites/arrows/line_udlr.png"
#define PATH_TEXTURE_ARROW_UD                   "assets/sprites/arrows/arrow_ud.png"

#define PATH_TEXTURE_UPGRADE_ADD_PAWN           "assets/sprites/upgrades/node_add_pawn.png"
#define PATH_TEXTURE_UPGRADE_EXPAND_BOARD       "assets/sprites/upgrades/node_expand_board.png"
#define PATH_TEXTURE_UPGRADE_ADD_LANCE          "assets/sprites/upgrades/node_add_lance.png"
#define PATH_TEXTURE_UPGRADE_SACRIFICE          "assets/sprites/upgrades/node_sacrifice.png"

#define PATH_TEXTURE_TREE_BUTTON_IDLE           "assets/sprites/tile/tile_selected.png"
#define PATH_TEXTURE_TREE_BUTTON_HOVERED        "assets/sprites/tile/tile_valid.png"
#define PATH_TEXTURE_TREE_BUTTON_PRESSED        "assets/sprites/tile/tile_capture.png"

#define PATH_TEXTURE_WINDOW_TREE_BACKGROUND     "assets/sprites/menus/window.png"

#define PATH_SOUND_CLICK                        "assets/sounds/click.wav"
#define PATH_SOUND_ERROR                        "assets/sounds/error.wav"

typedef enum TreeTextures
{
    TEXTURE_LINE_UD,
    TEXTURE_LINE_LR,
    TEXTURE_LINE_DL,
    TEXTURE_LINE_DR,
    TEXTURE_LINE_UL,
    TEXTURE_LINE_UR,
    TEXTURE_LINE_UDLR,
    TEXTURE_ARROW_UD,

    TEXTURE_UPGRADE_ADD_PAWN,
    TEXTURE_UPGRADE_EXPAND_BOARD,
    TEXTURE_UPGRADE_ADD_LANCE,
    TEXTURE_UPGRADE_SACRIFICE,

    TEXTURE_TREE_BUTTON_IDLE,
    TEXTURE_TREE_BUTTON_HOVERED,
    TEXTURE_TREE_BUTTON_PRESSED,

    TREE_TEXTURES_COUNT
} TreeTextures;

static const char* tree_textures[] = 
{
    PATH_TEXTURE_LINE_UD,
    PATH_TEXTURE_LINE_LR,
    PATH_TEXTURE_LINE_DL,
    PATH_TEXTURE_LINE_DR,
    PATH_TEXTURE_LINE_UL,
    PATH_TEXTURE_LINE_UR,
    PATH_TEXTURE_LINE_UDLR,
    PATH_TEXTURE_ARROW_UD,

    PATH_TEXTURE_UPGRADE_ADD_PAWN,
    PATH_TEXTURE_UPGRADE_EXPAND_BOARD,
    PATH_TEXTURE_UPGRADE_ADD_LANCE,
    PATH_TEXTURE_UPGRADE_SACRIFICE,

    PATH_TEXTURE_TREE_BUTTON_IDLE,
    PATH_TEXTURE_TREE_BUTTON_HOVERED,
    PATH_TEXTURE_TREE_BUTTON_PRESSED,
};
