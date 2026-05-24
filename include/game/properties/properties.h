#pragma once

#include <SDL3/SDL_stdinc.h>
#include <stdarg.h>
#include "game/board/board_elements/piece.h"
#include "game/board/board_elements/tile.h"
#include "game/player/player.h"
#include "game/rules/rules.h"

// ========== piece ==========
const char* get_piece_properties(const Piece* piece);

// ========== tile ==========
const char* get_tile_properties(const Tile* tile);

// ========== player ==========
const char* get_player_properties(const Player* player);
