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

static void tree_add_upgrade(Tree* tree, UpgradeType type, int cost);
static void tree_add_upgrade_dependency(Tree* tree, int upgrade_index, int dependency_index);
static void tree_upgrade_unlock(Tree* tree, int index);

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

void tree_set_default(Tree* tree)
{
    tree_add_upgrade(tree, UPGRADE_PEASANT, 1); // 0
    tree_upgrade_unlock(tree, 0);
    tree_add_upgrade(tree, UPGRADE_PEASANT, 1); // 1
    tree_add_upgrade_dependency(tree, 1, 0);

    tree_add_upgrade(tree, UPGRADE_SHOGI, 3); // 2
    tree_add_upgrade_dependency(tree, 2, 1);
    tree_add_upgrade(tree, UPGRADE_SHOGI, 3); // 3
    tree_add_upgrade_dependency(tree, 3, 2);

    tree_add_upgrade(tree, UPGRADE_CHECKERS, 3); // 4
    tree_add_upgrade_dependency(tree, 4, 1);
    tree_add_upgrade(tree, UPGRADE_CHECKERS, 3); // 5
    tree_add_upgrade_dependency(tree, 5, 4);


    tree_add_upgrade(tree, UPGRADE_EXPANSION, 1); // 6
    tree_upgrade_unlock(tree, 6);
    tree_add_upgrade(tree, UPGRADE_EXPANSION, 1); // 7
    tree_add_upgrade_dependency(tree, 7, 6);
    tree_add_upgrade(tree, UPGRADE_EXPANSION, 1); // 8
    tree_add_upgrade_dependency(tree, 8, 7);

    tree_add_upgrade(tree, UPGRADE_RISE, 1); // 9
    tree_add_upgrade_dependency(tree, 9, 6);
    tree_add_upgrade(tree, UPGRADE_RISE, 1); // 10
    tree_add_upgrade_dependency(tree, 10, 9);

    tree_add_upgrade(tree, UPGRADE_SWAMP, 3); // 11
    tree_add_upgrade_dependency(tree, 11, 9);
    tree_add_upgrade(tree, UPGRADE_SWAMP, 3); // 12
    tree_add_upgrade_dependency(tree, 12, 11);
}

static void tree_add_upgrade(Tree* tree, UpgradeType type, int cost)
{
    Upgrade* upgrade = SDL_malloc(sizeof(Upgrade));
    upgrade->cost = cost;
    upgrade->type = type;
    upgrade->state = LOCKED;
    graph_add_node(tree, upgrade);
}

static void tree_add_upgrade_dependency(Tree* tree, int upgrade_index, int dependency_index)
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

UpgradeType tree_upgrade_purchase(Tree* tree, int index)
{
    const Vector* next = graph_get_next_data(tree, index);
    for (int i = 0; i < vector_get_size(next); i++)
    {
        Upgrade* curr = vector_get_at(next, i);
        int curr_index = graph_search(tree, curr);
            curr->state = UNLOCKED;
    }
    Upgrade* upgrade = graph_get_data(tree, index);
    upgrade->state = PURCHASED;
    return upgrade->type;
}

static void tree_upgrade_unlock(Tree* tree, int index)
{
    Upgrade* upgrade = graph_get_data(tree, index);
    upgrade->state = UNLOCKED;
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

bool tree_is_upgrade_available(const Tree* tree, int index)
{
    Upgrade* upgrade = graph_get_data(tree, index);
    return upgrade->state == UNLOCKED;
}

