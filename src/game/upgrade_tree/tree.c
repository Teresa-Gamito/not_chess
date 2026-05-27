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
    tree_add_upgrade(tree, UPGRADE_EXPANSION, 1); // 1
    tree_upgrade_unlock(tree, 1);
    tree_add_upgrade(tree, UPGRADE_TACTICAL_ADVANTAGE, 1); // 2
    tree_upgrade_unlock(tree, 2);
    tree_add_upgrade(tree, UPGRADE_SHOGI, 1); // 3
    tree_add_upgrade_dependency(tree, 3, 0);
    tree_add_upgrade(tree, UPGRADE_PEASANT, 1); // 4
    tree_add_upgrade_dependency(tree, 4, 0);
    tree_add_upgrade(tree, UPGRADE_EXPANSION, 1); // 5
    tree_add_upgrade_dependency(tree, 5, 1);
    tree_add_upgrade(tree, UPGRADE_PRECIOUS, 1); // 6
    tree_add_upgrade_dependency(tree, 6, 2);
    tree_add_upgrade(tree, UPGRADE_SHOGI, 1); // 7
    tree_add_upgrade_dependency(tree, 7, 3);
    tree_add_upgrade(tree, UPGRADE_PEASANT, 1); // 8
    tree_add_upgrade_dependency(tree, 8, 4);
    tree_add_upgrade(tree, UPGRADE_LANCER, 3); // 9
    tree_add_upgrade_dependency(tree, 9, 7);
    tree_add_upgrade(tree, UPGRADE_DISGUISE, 3); // 10
    tree_add_upgrade_dependency(tree, 10, 0);
    tree_add_upgrade(tree, UPGRADE_REVERSE_DISGUISE, 3); // 11
    tree_add_upgrade_dependency(tree, 11, 0);
    tree_add_upgrade(tree, UPGRADE_RISE, 3); // 12
    tree_add_upgrade_dependency(tree, 12, 5);
    tree_add_upgrade(tree, UPGRADE_PRECIOUS, 3); // 13
    tree_add_upgrade_dependency(tree, 13, 6);
    tree_add_upgrade(tree, UPGRADE_FRIENDLY_FIRE, 3); // 14
    tree_add_upgrade_dependency(tree, 14, 2);
    tree_add_upgrade(tree, UPGRADE_LANCER, 3); // 15
    tree_add_upgrade_dependency(tree, 15, 9);
    tree_add_upgrade(tree, UPGRADE_RISE, 3); // 16
    tree_add_upgrade_dependency(tree, 16, 12);
    tree_add_upgrade(tree, UPGRADE_PROMOTION, 5); // 17
    tree_add_upgrade_dependency(tree, 17, 10);
    tree_add_upgrade(tree, UPGRADE_PROMOTION, 5); // 18
    tree_add_upgrade_dependency(tree, 18, 11);
    tree_add_upgrade(tree, UPGRADE_PRECIOUS, 5); // 19
    tree_add_upgrade_dependency(tree, 19, 13);
    tree_add_upgrade(tree, UPGRADE_GAMBLING, 5); // 20
    tree_add_upgrade_dependency(tree, 20, 2);
    tree_add_upgrade(tree, UPGRADE_ROYALTY, 7); // 21
    tree_add_upgrade_dependency(tree, 21, 18);
    tree_add_upgrade_dependency(tree, 21, 19);
    tree_add_upgrade(tree, UPGRADE_PRECIOUS, 7); // 22
    tree_add_upgrade_dependency(tree, 22, 20);
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

bool tree_is_upgrade_purchased(const Tree* tree, int index)
{
    Upgrade* upgrade = graph_get_data(tree, index);
    return upgrade->state == PURCHASED;
}

const char* upgrade_get_name(UpgradeType type)
{
    return upgrade_name[type];
}

const char* upgrade_get_description(UpgradeType type)
{
    return upgrade_description[type];
}
