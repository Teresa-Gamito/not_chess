// tiles on the board
// responsible for its own properties
#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include "include/helper_functions/error_handling.h"
#include "helper_functions/typeops.h"
#include "game/board/color.h"

typedef struct Tile Tile;

typedef enum TileType
{
    TILE_NONE,
    TILE_TYPE_COUNT
} TileType;

Tile* tile_create(TileType type, Color color);

void tile_destroy(Tile* tile);

TileType tile_get_type(const Tile* tile);
Color tile_get_color(const Tile* tile);
Color tile_get_default_color(int col, int row);

TypeOps* tile_ops();

void verify_tile(const Tile* piece);
void verify_tile_type(TileType type);
