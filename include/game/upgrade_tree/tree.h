#pragma once

#include <SDL3/SDL_stdinc.h>
#include "data_structures/graph.h"
#include "game/board/board.h"

typedef Graph Tree;

typedef enum UpgradeType
{
    UPGRADE_PEASANT,
    UPGRADE_CHECKERS,
    UPGRADE_STONKS,
    UPGRADE_SHOGI,
    UPGRADE_EXPANSION,
    UPGRADE_MORE_LAND,
    UPGRADE_ONLY_ONE,
    UPGRADE_KNIGHTHOOD,
    UPGRADE_DISGUISE,
    UPGRADE_REVERSE_DISGUISE,
    UPGRADE_PROMOTION,
    UPGRADE_PRECIOUS,
    UPGRADE_DEADTOUCH,
    UPGRADE_TRAMPLE,
    UPGRADE_DEBUFF,
    UPGRADE_TEMPORARY_ROYALTY,
    UPGRADE_AGAIN,
    UPGRADE_RISE,
    UPGRADE_SWAMP,
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

void tree_add_upgrade(Tree* tree, UpgradeType type, int cost);
void tree_add_upgrade_dependency(Tree* tree, int upgrade_index, int dependency_index);

void tree_upgrade_unlock(Tree* tree, int index);
void tree_upgrade_purchase(Board* board, Tree* tree, int index);

int tree_get_upgrade_cost(const Tree* tree, int index);
UpgradeType tree_get_upgrade_type(const Tree* tree, int index);
