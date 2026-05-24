#pragma once

#include <SDL3/SDL_stdinc.h>
#include "game/color.h"
#include "game/player/player.h"
#include "game/board/board.h"
#include "game/board/board_elements/tile.h"
#include "game/board/board_elements/piece.h"
#include "data_structures/queue.h"

typedef enum UpgradeType
{
    UPGRADE_PEASANT, // done
    UPGRADE_CHECKERS,
    UPGRADE_STONKS,
    UPGRADE_SHOGI, // done
    UPGRADE_EXPANSION, // done
    UPGRADE_MORE_LAND, // done
    UPGRADE_ONLY_ONE,
    UPGRADE_KNIGHTHOOD,
    UPGRADE_DISGUISE, // done
    UPGRADE_REVERSE_DISGUISE, // done
    UPGRADE_PROMOTION, // done
    UPGRADE_PRECIOUS, // done
    UPGRADE_DEADTOUCH,
    UPGRADE_TRAMPLE,
    UPGRADE_DEBUFF,
    UPGRADE_TEMPORARY_ROYALTY,
    UPGRADE_AGAIN,
    UPGRADE_RISE, // done
    UPGRADE_SWAMP, // done
    UPGRADE_RULING,
    UPGRADE_GAMBLING,
    UPGRADE_HELLO,
    UPGRADE_FRIENDLY_FIRE,
    UPGRADE_TACTICAL_ADVANTAGE,
    UPGRADE_RUN,
    COUNT_UPGRADE,
} UpgradeType;

bool upgrade_needs_select(UpgradeType type);
bool upgrade_pos_is_valid(UpgradeType type, Board* board, Pos pos, Player* active_player);
void upgrade(UpgradeType type, Board* board, Pos pos, Player* active_player);
