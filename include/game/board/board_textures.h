#pragma once

#define TEXTURE_DEFAULT_SIZE_PX 16

#define PATH_TEXTURE_WHITE_PAWN                 "assets/sprites/white_pawn.png"
#define PATH_TEXTURE_WHITE_ROOK                 "assets/sprites/white_rook.png"
#define PATH_TEXTURE_WHITE_KNIGHT               "assets/sprites/white_knight.png"
#define PATH_TEXTURE_WHITE_BISHOP               "assets/sprites/white_bishop.png"
#define PATH_TEXTURE_WHITE_QUEEN                "assets/sprites/white_queen.png"
#define PATH_TEXTURE_WHITE_KING                 "assets/sprites/white_king.png"
#define PATH_TEXTURE_WHITE_LANCE                "assets/sprites/white_lance.png"

#define PATH_TEXTURE_BLACK_PAWN                 "assets/sprites/black_pawn.png"  
#define PATH_TEXTURE_BLACK_ROOK                 "assets/sprites/black_rook.png"  
#define PATH_TEXTURE_BLACK_KNIGHT               "assets/sprites/black_knight.png"
#define PATH_TEXTURE_BLACK_BISHOP               "assets/sprites/black_bishop.png"
#define PATH_TEXTURE_BLACK_QUEEN                "assets/sprites/black_queen.png" 
#define PATH_TEXTURE_BLACK_KING                 "assets/sprites/black_king.png"  
#define PATH_TEXTURE_BLACK_LANCE                "assets/sprites/black_lance.png"

#define PATH_TEXTURE_TILE_WHITE                 "assets/sprites/tile_white.png"
#define PATH_TEXTURE_TILE_BLACK                 "assets/sprites/tile_black.png"

#define PATH_TEXTURE_TILE_SELECTED              "assets/sprites/tile_selected.png"
#define PATH_TEXTURE_TILE_VALID                 "assets/sprites/tile_valid.png"
#define PATH_TEXTURE_TILE_CAPTURE               "assets/sprites/tile_capture.png"

#define PATH_TEXTURE_WINDOW_BOARD_BACKGROUND    "assets/sprites/tile_black.png"

typedef enum BoardTextures
{
    TEXTURE_WHITE_PAWN,
    TEXTURE_WHITE_ROOK,
    TEXTURE_WHITE_KNIGHT,
    TEXTURE_WHITE_BISHOP,
    TEXTURE_WHITE_QUEEN,
    TEXTURE_WHITE_KING,
    TEXTURE_WHITE_LANCE,
    

    TEXTURE_BLACK_PAWN,
    TEXTURE_BLACK_ROOK,
    TEXTURE_BLACK_KNIGHT,
    TEXTURE_BLACK_BISHOP,
    TEXTURE_BLACK_QUEEN,
    TEXTURE_BLACK_KING,
    TEXTURE_BLACK_LANCE,

    TEXTURE_TILE_WHITE,
    TEXTURE_TILE_BLACK,

    TEXTURE_TILE_SELECTED,
    TEXTURE_TILE_VALID,
    TEXTURE_TILE_CAPTURE,

    BOARD_TEXTURE_COUNT
} BoardTextures;

static const char* board_textures[] =
{
    PATH_TEXTURE_WHITE_PAWN,
    PATH_TEXTURE_WHITE_ROOK,
    PATH_TEXTURE_WHITE_KNIGHT,
    PATH_TEXTURE_WHITE_BISHOP,
    PATH_TEXTURE_WHITE_QUEEN,
    PATH_TEXTURE_WHITE_KING,
    PATH_TEXTURE_WHITE_LANCE,

    PATH_TEXTURE_BLACK_PAWN,
    PATH_TEXTURE_BLACK_ROOK,
    PATH_TEXTURE_BLACK_KNIGHT,
    PATH_TEXTURE_BLACK_BISHOP,
    PATH_TEXTURE_BLACK_QUEEN,
    PATH_TEXTURE_BLACK_KING,
    PATH_TEXTURE_BLACK_LANCE,

    PATH_TEXTURE_TILE_WHITE,
    PATH_TEXTURE_TILE_BLACK,

    PATH_TEXTURE_TILE_SELECTED,
    PATH_TEXTURE_TILE_VALID,
    PATH_TEXTURE_TILE_CAPTURE
};
