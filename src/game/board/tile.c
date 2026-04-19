#include "../../../include/game/board/tile.h"

typedef enum TileUI
{
    TILEUI_NONE,
    TILEUI_SELECTED,
    TILEUI_VALID,
    TILEUI_CAPTURE
} TileUI;

struct Tile
{
    TileType type;

    TileUI ui;
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

    tile->ui = TILEUI_NONE;
    tile->object = NULL;
    tile->button = NULL;

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
void tile_set_ui_textures(
    Tile* tile, 
    SDL_Texture* texture_selected, 
    SDL_Texture* texture_valid, 
    SDL_Texture* texture_capture
)
{
    switch (tile->ui)
    {
        case TILEUI_SELECTED:
            tile_set_texture(tile, texture_selected);
            break;

        case TILEUI_VALID:
            tile_set_texture(tile, texture_valid);
            break;

        case TILEUI_CAPTURE:
            tile_set_texture(tile, texture_capture);
            break;

        case TILEUI_NONE:
        default:
            tile_set_texture(tile, NULL);
            break;
    }
}
void tile_ui_set_none(Tile* tile)
{
    tile->ui = TILEUI_NONE;
}
void tile_ui_set_selected(Tile* tile)
{
    tile->ui = TILEUI_SELECTED;
}
void tile_ui_set_valid(Tile* tile)
{
    tile->ui = TILEUI_VALID;
}
void tile_ui_set_capture(Tile* tile)
{
    tile->ui = TILEUI_CAPTURE;
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
bool tile_is_selected(const Tile* tile)
{
    verify(tile == NULL, "Tile does not exist");

    return tile->ui == TILEUI_SELECTED;
}
bool tile_is_valid(const Tile* tile)
{
    verify(tile == NULL, "Tile does not exist");

    return tile->ui == TILEUI_VALID;
}
bool tile_is_capture(const Tile* tile)
{
    verify(tile == NULL, "Tile does not exist");

    return tile->ui == TILEUI_CAPTURE;
}
