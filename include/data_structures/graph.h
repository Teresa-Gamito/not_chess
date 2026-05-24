#pragma once

#include <SDL3/SDL_stdinc.h>
#include "data_structures/stack.h"

typedef struct Graph Graph;

Graph* graph_create();
void graph_destroy(Graph* graph);
void graph_add_node(Graph* graph, void* data);
void graph_add_edge(Graph* graph, int index_from, int index_to);

void* graph_get_data(const Graph* graph, int index);
int graph_get_size(const Graph* graph);
int graph_search(const Graph* graph, const void* data);
const Vector* graph_get_next_data(const Graph* graph, int index);
const Vector* graph_get_prev_data(const Graph* graph, int index);
bool graph_is_connected(const Graph* graph, int index1, int index2);

void verify_graph(const Graph* graph);
