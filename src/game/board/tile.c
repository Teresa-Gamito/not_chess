#include "include/game/board/tile.h"
#include "helper_functions/error_handling.h"

struct Tile
{
    TileType type;
    Color color;
};

Tile* tile_create(TileType type, Color color)
{
    verify_tile_type(type);
    verify(color != WHITE && color != BLACK, "Invalid Color");

    Tile* tile = (Tile*)SDL_malloc(sizeof(Tile));
    verify(tile == NULL, "Tile could not be created: malloc");

    tile->type = type;
    tile->color = color;

    return tile;
}

void tile_destroy(Tile* tile)
{
    verify_tile(tile);

    SDL_free(tile);
}

TileType tile_get_type(const Tile* tile)
{
    verify_tile(tile);

    return tile->type;
}
Color tile_get_color(const Tile* tile)
{
    verify_tile(tile);

    return tile->color;
}
Color tile_get_default_color(int col, int row)
{
    return ((col + row) % 2 == 0) ? WHITE : BLACK;
}



static void destroy(void* tile)
{
    tile_destroy(tile);
}
static TypeOps ops =
    {
        destroy
    };
TypeOps* tile_ops()
{
    return &ops;
}



void verify_tile(const Tile* tile)
{
    verify(tile == NULL, "Tile does not exist");
}
void verify_tile_type(TileType type)
{
    verify(type < 0 || type >= TILE_TYPE_COUNT, "Invalid TileType");
}
