#pragma once

#include <SDL3/SDL_stdinc.h>
#include "helper_functions/graph.h"

typedef enum UpgradeIndex
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
} UpgradeIndex;

Graph* tree_create();
void tree_destroy(Graph *tree);

void tree_set_default(Graph *tree);

void tree_node_unlock(Graph* tree, UpgradeIndex node);
void tree_node_purchase(Graph* tree, UpgradeIndex node);
