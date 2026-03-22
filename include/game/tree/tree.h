#pragma once

#include "node.h"

typedef struct Tree Tree;

Tree *create_tree();
void destroy_tree(Tree *tree);

void tree_add_node(Tree *tree, Node *node);

