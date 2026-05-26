#pragma once

#include <SDL3/SDL_stdinc.h>
#include <stdarg.h>
#include "game/board/board_elements/piece.h"
#include "game/board/board_elements/tile.h"
#include "game/player/player.h"
#include "game/rules/rules.h"
#include "game/upgrade_tree/tree.h"

const char* get_piece_properties(const Piece* piece);
const char* get_tile_properties(const Tile* tile);
const char* get_player_properties(const Player* player);
const char* get_upgrade_properties(const Tree* tree, int index);
