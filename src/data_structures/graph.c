#include "include/data_structures/graph.h"
#include "data_structures/vector.h"

typedef struct Node
{
    void* data;
    Vector* next_nodes;
    Vector* prev_nodes;
} Node;

struct Graph
{
    Vector* nodes;
};

static Node* node_create(void* data)
{
    Node* node = SDL_malloc(sizeof(Node));
    verify(node == NULL, "Node could not be created: malloc");

    node->data = data;
    node->prev_nodes = vector_create();
    node->next_nodes = vector_create();

    return node;
}

static void node_add_edge(Node* from, Node* to)
{
    vector_add(from->next_nodes, to);
    vector_add(to->prev_nodes, from);
}

static void node_destroy(Node* node)
{
    vector_destroy(node->prev_nodes);
    vector_destroy(node->next_nodes);

    SDL_free(node);
}



Graph* graph_create()
{
    Graph* graph = SDL_malloc(sizeof(Graph));
    verify(graph == NULL, "Graph could not be created: malloc");

    graph->nodes = vector_create();

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
    return vector_get_at(graph->nodes, index);
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

int graph_search(const Graph* graph, const void* data)
{
    for (int i = 0; i < vector_get_size(graph->nodes); i++)
    {
        if (graph_get_data(graph, i) == data)
        {
            return i;
        }
    }
    return -1;
}

int graph_get_size(const Graph* graph)
{
    return vector_get_size(graph->nodes);
}

const Vector* graph_get_next_data(const Graph* graph, int index)
{
    Node* node = graph_get_node(graph, index);
    return node->next_nodes;
}

const Vector* graph_get_prev_data(const Graph* graph, int index)
{
    Node* node = graph_get_node(graph, index);
    return node->prev_nodes;
}

void verify_graph(const Graph* graph)
{
    verify(graph == NULL, "Graph does not exist");
}
