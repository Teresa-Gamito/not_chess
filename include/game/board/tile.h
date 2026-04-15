#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include "../../helper_functions/error_handling.h"
#include "../../ui_elements/object.h"
#include "../../ui_elements/button.h"

typedef struct Tile Tile;

typedef enum TileType
{
    TILE_NONE,
    TILE_TYPE_COUNT
} TileType;



// ========== create ==========
Tile* tile_create(TileType type);



// ========== destroy ==========
void tile_destroy(Tile* tile);



// ========== set ==========
void tile_set_type(Tile* tile, TileType type);
void tile_set_object(Tile* tile, Object* object);
void tile_set_button(Tile* tile, Button* button);
void tile_set_texture(Tile* tile, SDL_Texture* texture);
void tile_set_ui(
    Tile* tile, 
    SDL_Texture* texture_selected, 
    SDL_Texture* texture_valid, 
    SDL_Texture* texture_capture
);
void tile_ui_set_none(Tile* tile);
void tile_ui_set_selected(Tile* tile);
void tile_ui_set_valid(Tile* tile);
void tile_ui_set_capture(Tile* tile);



// ========== get ==========
TileType tile_get_type(const Tile* tile);
Object* tile_get_object(const Tile* tile);
Button* tile_get_button(const Tile* tile);
