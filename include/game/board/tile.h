#pragma once

#include <SDL3/SDL_stdinc.h>

typedef struct Tile Tile;

typedef enum TileType
{
    TILE_NONE
} TileType;



// ========== create ==========
Tile* tile_create(TileType type);



// ========== destroy ==========
void tile_destroy(Tile* tile);



// ========== set ==========
void tile_set_type(Tile* tile, TileType type);



// ========== get ==========
TileType tile_get_type(const Tile* tile);
