#pragma once

#include <SDL3/SDL_stdinc.h>
#include "include/game/board/piece.h"
#include "include/game/board/tile.h"


const char* get_piece_properties(const Piece* piece);
const char* get_tile_properties(const Tile* tile);
const char* get_rules();
const char* get_log();
