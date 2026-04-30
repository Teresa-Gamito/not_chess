// tiles on the board
// responsible for its own properties
#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include "helper_functions/typeops.h"
#include "include/helper_functions/error_handling.h"

typedef struct Tile Tile;

typedef enum TileType
{
    TILE_NONE,
    TILE_TYPE_COUNT
} TileType;

Tile* tile_create(TileType type);

void tile_destroy(Tile* tile);

void tile_set_type(Tile* tile, TileType type);

TileType tile_get_type(const Tile* tile);

TypeOps* tile_ops();
