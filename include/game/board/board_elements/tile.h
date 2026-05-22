#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include "game/color.h"
#include "helper_functions/position.h"
#include "helper_functions/error_handling.h"

typedef struct Tile Tile;

typedef enum TileType
{
    TILE_NONE,
    TILE_WALL,
    TILE_POISON,

    TILE_TYPE_COUNT
} TileType;

// ========== create ==========
Tile* tile_create(TileType type, Color color);

// ========== destroy ==========
void tile_destroy(Tile* tile);

// ========== set ==========
void tile_set_type(Tile* tile, TileType type); // TODO:

// ========== get ==========
TileType tile_get_type(const Tile* tile);
Color tile_get_color(const Tile* tile);
Color tile_get_default_color(Pos pos);

// ========== verify ==========
void verify_tile(const Tile* piece);
void verify_tile_type(TileType type);
