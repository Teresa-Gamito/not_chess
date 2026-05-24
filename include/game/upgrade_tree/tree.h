#pragma once

#include <SDL3/SDL_stdinc.h>
#include "data_structures/graph.h"

typedef Graph Tree;

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

Tree* tree_create();
void tree_destroy(Tree *tree);

void tree_set_default(Tree* tree);

UpgradeType tree_upgrade_purchase(Tree* tree, int index);

int tree_get_upgrade_cost(const Tree* tree, int index);
UpgradeType tree_get_upgrade_type(const Tree* tree, int index);
bool tree_is_upgrade_available(const Tree* tree, int index);
