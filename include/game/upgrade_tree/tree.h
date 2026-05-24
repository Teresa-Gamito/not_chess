#pragma once

#include <SDL3/SDL_stdinc.h>
#include "data_structures/graph.h"
#include "game/board/board.h"
#include "game/upgrade_tree/upgrade.h"

typedef Graph Tree;

Tree* tree_create();
void tree_destroy(Tree *tree);

void tree_set_default(Tree* tree);

void tree_add_upgrade(Tree* tree, UpgradeType type, int cost);
void tree_add_upgrade_dependency(Tree* tree, int upgrade_index, int dependency_index);

void tree_upgrade_unlock(Tree* tree, int index);
void tree_upgrade_purchase(Board* board, Tree* tree, int index);

int tree_get_upgrade_cost(const Tree* tree, int index);
UpgradeType tree_get_upgrade_type(const Tree* tree, int index);
