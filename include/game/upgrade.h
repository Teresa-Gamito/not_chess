#pragma once

#include <SDL3/SDL_stdinc.h>
#include "game/game.h"

bool upgrade_needs_select(UpgradeType type);
bool upgrade_pos_is_valid(Game* game, UpgradeType type, Pos pos);
void upgrade(Game* game, UpgradeType type, Pos pos);
