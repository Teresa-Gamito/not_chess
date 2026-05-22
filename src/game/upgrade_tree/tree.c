#include "game/upgrade_tree/tree.h"

typedef enum UpgradeState
{
    LOCKED,
    UNLOCKED,
    PURCHASED
} UpgradeState;

typedef struct Upgrade
{
    int cost;
    UpgradeType type;
    UpgradeState state;
} Upgrade;

Tree* tree_create()
{
    return graph_create();
}

void tree_destroy(Tree *tree)
{
    for (int i = 0; i < graph_get_size(tree); i++)
    {
        Upgrade* upgrade = graph_get_data(tree, i);
        SDL_free(upgrade);
    }
    graph_destroy(tree);
}

void tree_add_upgrade(Tree* tree, UpgradeType type, int cost)
{
    Upgrade* upgrade = SDL_malloc(sizeof(Upgrade));
    upgrade->cost = cost;
    upgrade->type = type;
    upgrade->state = LOCKED;
    graph_add_node(tree, upgrade);
}

void tree_add_upgrade_dependency(Tree* tree, int upgrade_index, int dependency_index)
{
    graph_add_edge(tree, dependency_index, upgrade_index);
}

static bool upgrade_can_unlock(Tree* tree, int index)
{
    const Vector* prev = graph_get_prev_data(tree, index);
    for (int i = 0; i < vector_get_size(prev); i++)
    {
        Upgrade* curr = vector_get_at(prev, i);
        if (curr->state != PURCHASED)
        {
            return false;
        }
    }
    return true;
}

void tree_upgrade_purchase(Board* board, Tree* tree, int index)
{
    const Vector* next = graph_get_next_data(tree, index);
    for (int i = 0; i < vector_get_size(next); i++)
    {
        Upgrade* curr = vector_get_at(next, i);
        int curr_index = graph_search(tree, curr);
        if (upgrade_can_unlock(tree, curr_index))
        {
            curr->state = UNLOCKED;
        }
    }

    // TODO: do the effects of the upgrade
}

int tree_get_upgrade_cost(const Tree* tree, int index)
{
    Upgrade* upgrade = graph_get_data(tree, index);
    return upgrade->cost;
}

UpgradeType tree_get_upgrade_type(const Tree* tree, int index)
{
    Upgrade* upgrade = graph_get_data(tree, index);
    return upgrade->type;
}
