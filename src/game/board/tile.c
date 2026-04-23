#include "include/game/board/tile.h"

struct Tile
{
    TileType type;
};

Tile* tile_create(TileType type)
{
    verify(type < 0 || type >= TILE_TYPE_COUNT, "Invalid TileType");

    Tile* tile = (Tile*)SDL_malloc(sizeof(Tile));
    verify(tile == NULL, "Tile could not be created: malloc");

    tile->type = type;

    return tile;
}

void tile_destroy(Tile* tile)
{
    SDL_free(tile);
}

void tile_set_type(Tile* tile, TileType type)
{
    verify(tile == NULL, "Tile does not exist");
    verify(type < 0 || type >= TILE_TYPE_COUNT, "Invalid TileType");

    tile->type = type;
}

TileType tile_get_type(const Tile* tile)
{
    return tile->type;
}
