#pragma once

#include <SDL3/SDL_stdinc.h>
#include "helper_functions/vector.h"
#include "helper_functions/typeops.h"

typedef struct Graph Graph;

Graph* graph_create();
void graph_destroy(Graph* graph);
void graph_add_node(Graph* graph, void* data);
void graph_add_edge(Graph* graph, int index_from, int index_to);
