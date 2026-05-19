#include "game/upgrade_tree/tree.h"
#include "helper_functions/graph.h"

typedef enum NodeState
{
    LOCKED,
    UNLOCKED,
    PURCHASED
} NodeState;

typedef struct Node 
{
    char* name;
    char* description;
    int cost;
    NodeState state;
} Node;



static Node* node_create(const char* name, const char* description, int cost)
{
    Node* node = SDL_malloc(sizeof(Node));
    verify(node == NULL, "Node could not be created: malloc");

    node->name = SDL_strdup(name);
    node->description = SDL_strdup(description);
    node->cost = cost;
    node->state = LOCKED;

    return node;
}

static void node_destroy(Node* node)
{
    SDL_free(node->name);
    SDL_free(node->description);
    SDL_free(node);
}

Graph* tree_create()
{
    return graph_create();
}
void tree_destroy(Graph *tree)
{

}

void tree_node_purchase(Graph* tree, NodeIndex index)
{
    Node* node = graph_get_data(tree, index);
    node->state = PURCHASED;

}

void tree_set_default(Graph *tree);
