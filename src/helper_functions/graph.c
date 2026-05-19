#include "include/helper_functions/graph.h"
#include "helper_functions/vector.h"

typedef struct Node
{
    void* data;
    Vector* next_nodes;
    Vector* prev_nodes;
    TypeOps* ops;
} Node;

struct Graph
{
    Vector* nodes;
};


TypeOps* node_ops();


static Node* node_create(void* data)
{
    Node* node = SDL_malloc(sizeof(Node));
    verify(node == NULL, "Node could not be created: malloc");

    node->data = data;
    node->prev_nodes = vector_create(node_ops());
    node->next_nodes = vector_create(node_ops());

    return node;
}

static void node_add_edge(Node* from, Node* to)
{
    vector_add(from->next_nodes, (void*) to);
    vector_add(to->prev_nodes, (void*) from);
}

static void node_destroy(Node* node)
{

    for (int i = 0; i < vector_get_count(node->prev_nodes); i++)
    {
        vector_set_at(node->prev_nodes, NULL, i);
    }
    vector_destroy(node->prev_nodes);

    for (int i = 0; i < vector_get_count(node->prev_nodes); i++)
    {
        vector_set_at(node->prev_nodes, NULL, i);
    }
    vector_destroy(node->next_nodes);

    SDL_free(node);
}



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

static Node* graph_get_node(const Graph* graph, int index)
{
    if (index >= vector_get_count(graph->nodes))
    {
        return NULL;
    }
    return (Node*) vector_get_at(graph->nodes, index);
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
