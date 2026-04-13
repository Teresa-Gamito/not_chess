#include "../../../include/game/board/tile.h"

struct Tile
{
    TileType type;

    Object* object;
    Button* button;
};



// ========== create ==========
Tile* tile_create(TileType type)
{
    verify(type < 0 || type >= TILE_TYPE_COUNT, "Invalid TileType");

    Tile* tile = (Tile*)SDL_malloc(sizeof(Tile));
    verify(tile == NULL, "Tile could not be created: malloc");

    tile->type = type;

    tile->object = NULL;

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
    verify(tile == NULL, "Tile does not exist");
    verify(type < 0 || type >= TILE_TYPE_COUNT, "Invalid TileType");

    tile->type = type;
}
void tile_set_object(Tile* tile, Object* object)
{
    verify(tile == NULL, "Tile does not exist");

    tile->object = object;
}
void tile_set_button(Tile* tile, Button* button)
{
    verify(tile == NULL, "Tile does not exist");

    tile->button = button;
}
void tile_set_texture(Tile* tile, SDL_Texture* texture)
{
    verify(tile == NULL, "Tile does not exist");

    button_set_texture_all(tile->button, texture);
}


// ========== get ==========
TileType tile_get_type(const Tile* tile)
{
    return tile->type;
}
Object* tile_get_object(const Tile* tile)
{
    verify(tile == NULL, "Tile does not exist");

    return tile->object;
}
Button* tile_get_button(const Tile* tile)
{
    verify(tile == NULL, "Tile does not exist");

    return tile->button;
}
