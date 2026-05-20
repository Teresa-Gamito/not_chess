#include "game/upgrade_tree/tree.h"

typedef enum UpgradeState
{
    LOCKED,
    UNLOCKED,
    PURCHASED
} UpgradeState;

typedef struct Upgrade
{
    char* name;
    char* description;
    int cost;
    UpgradeState state;
} Upgrade;

static Upgrade* node_create(int cost)
{
    Upgrade* node = SDL_malloc(sizeof(Upgrade));
    verify(node == NULL, "Upgrade could not be created: malloc");

    node->name = SDL_strdup(name);
    node->description = SDL_strdup(description);
    node->cost = cost;
    node->state = LOCKED;

    return node;
}

static void node_destroy(Upgrade* node)
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

void tree_set_default(Graph *tree)
{
    Upgrade* upgrade_create();
}

void tree_node_purchase(Graph* tree, UpgradeIndex index)
{
    Upgrade* node = graph_get_data(tree, index);
    node->state = PURCHASED;
}

