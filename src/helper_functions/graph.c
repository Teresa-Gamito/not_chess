#include "include/game/helper_functions/graph.h"

typedef struct Node
{
    void* data;
    Vector* neighbours;
} Node;

struct Graph
{
    Vector* nodes;
};



static Node* graph_get_node(const Graph* graph, int index);

static Node* node_create(void* data);
static void node_add_edge(Node* node, const Node* neighbour);
static void node_destroy(Node* node);
TypeOps* node_ops();



Graph* graph_create()
{
    Graph* graph = SDL_malloc(sizeof(Graph));
    verify(graph == NULL, "Graph could not be created: malloc");

    graph->nodes = vector_create(node_ops());

    return graph;
}

void graph_destroy(Graph* graph)
{
    vector_destroy(graph->nodes);
    SDL_free(graph);
}

void graph_add_node(Graph* graph, void* data)
{
    Node* node = node_create(data);
    vector_add(graph->nodes, node);
}

void graph_add_edge(Graph* graph, int index_from, int index_to)
{
    Node* node = graph_get_node(graph, index_from);
    Node* neighbour = graph_get_node(graph, index_to);

    node_add_edge(node, neighbour);
}

void* graph_get_data(const Graph* graph, int index)
{
    Node* node = graph_get_node(graph, index);
    return node->data;
}

static Node* graph_get_node(const Graph* graph, int index)
{
    if (index >= vector_get_count(graph->nodes))
    {
        return NULL;
    }
    return (Node*) vector_get_at(graph->nodes, index);
}



static Node* node_create(void* data)
{
    Node* node = SDL_malloc(sizeof(Node));
    verify(node == NULL, "Node could not be created: malloc");

    node->data = data;
    node->neighbours = vector_create(node_ops());

    return node;
}

static void node_add_edge(Node* node, const Node* neighbour)
{
    vector_add(node->neighbours, (void*) neighbour);
}

static void node_destroy(Node* node)
{
    // destory data?
    SDL_free(node);
}



static void destroy(void* node)
{
    node_destroy(node);
}
static TypeOps ops =
    {
        destroy
    };
TypeOps* node_ops()
{
    return &ops;
}
