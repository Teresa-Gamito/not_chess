#pragma once

#include <SDL3/SDL_stdinc.h>
#include "helper_functions/graph.h"

typedef enum NodeIndex
{
    NODE_,

} NodeIndex;

Graph* tree_create();
void tree_destroy(Graph *tree);

void tree_set_default(Graph *tree);

void tree_node_unlock(Graph* tree, NodeIndex node);
void tree_node_purchase(Graph* tree, NodeIndex node);
