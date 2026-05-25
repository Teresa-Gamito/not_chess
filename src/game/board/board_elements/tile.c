#include "include/game/board/board_elements/tile.h"

struct Tile
{
    TileType type;
    Color color;
};

Tile* tile_create(TileType type, Color color)
{
    verify_tile_type(type);
    verify_color(color);

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

void tile_set_type(Tile* tile, TileType type)
{
    verify_tile(tile);
    verify_tile_type(type);
    tile->type = type;
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

Color tile_get_default_color(Pos pos)
{
    return ((pos.col + pos.row) % 2 == 0) ? WHITE : BLACK;
}

const char* tile_type_get_name(TileType type)
{
    switch (type) 
    {
        case TILE_NONE:
            return "Regular";
        case TILE_WALL:
            return "Wall";
        case TILE_POISON:
            return "POISON";

        default:
            return "Invalid TileType";
    }
}

void verify_tile(const Tile* tile)
{
    verify(tile == NULL, "Tile does not exist");
}

void verify_tile_type(TileType type)
{
    verify(type < 0 || type >= TILE_TYPE_COUNT, "Invalid TileType");
}
