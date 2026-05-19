// holder for nodes
#pragma once

#include <SDL3/SDL_stdinc.h>
#include "game/tree/node.h"

typedef struct Tree Tree;

Tree* tree_create();
void tree_set_default(Tree* tree);

void tree_destroy(Tree* tree);

void tree_add_node(Tree* tree, Node* node);
int tree_get_node_count(const Tree* tree);
Node* tree_get_node_at(const Tree* tree, int index);
