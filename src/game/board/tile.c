#include "../../../include/game/board/tile.h"

struct Tile
{
    TileType type;
};



// ========== create ==========
Tile* tile_create(TileType type)
{
    Tile* tile = (Tile*)SDL_malloc(sizeof(Tile));

    tile->type = type;

    return tile;
}



// ========== destroy ==========
void tile_destroy(Tile* tile)
{
    SDL_free(tile);
}



// ========== set ==========
void tile_set_type(Tile* tile, TileType type)
{
    tile->type = type;
}



// ========== get ==========
TileType tile_get_type(const Tile* tile)
{
    return tile->type;
}
