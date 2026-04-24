#include "include/game/tree/node.h"
#include "game/board/board.h"
#include "game/board/board_ui.h"
#include "game/board/player.h"
#include "helper_functions/error_handling.h"
#include "helper_functions/typeops.h"

typedef enum NodeState
{
    LOCKED,
    AVAILABLE,
    PURCHASED,
    NODE_STATE_COUNT
} NodeState;

struct Node
{
    NodeState state;
    Task* task;
    int cost;
};

Node* node_create(Task task, int cost)
{
    verify(task < 0 || task >= TASK_COUNT, "Invalid Task");
    verify(cost < 0, "Invalid cost");

    Node* node = SDL_malloc(sizeof(Node));
    verify(node == NULL, "Node could not be created: malloc");

    node->state = AVAILABLE;

    Task* task_to_add = SDL_malloc(sizeof(Task));
    *task_to_add = task;
    node->task = task_to_add;

    node->cost = cost;

    return node;
}
void node_destroy(Node* node)
{
    verify(node == NULL, "Node does not exist");

    SDL_free(node->task);
    SDL_free(node);
}

bool node_can_purchase(const Node* node, int player_points)
{
    verify(node == NULL, "Node does not exist");

    if (player_points < node->cost)
    {
        return false;
    }
    if (node->state != AVAILABLE)
    {
        return false;
    }
    return true;
}
void node_purchase(void* board_ui, void* node_to_purchase)
{
    BoardUI* ui = (BoardUI*)board_ui;
    Node* node = (Node*)node_to_purchase;

    verify(ui == NULL, "BoardUI does not exist");
    verify(node == NULL, "Node does not exist");

    Board* board = board_ui_get_board(ui);
    Player* player = board_get_active_player(board);

    if (!node_can_purchase(node, player_get_points(player)))
    {
        return;
    }

    board_ui_add_task(ui, *node->task);

    player_add_points(player, -node->cost);
    node->state = PURCHASED;
}
Task node_get_task(const Node* node)
{
    verify(node == NULL, "Node does not exist");

    return *node->task;
}
int node_get_cost(const Node* node)
{
    verify(node == NULL, "Node does not exist");

    return node->cost;
}

static void destroy(void* node)
{
    node_destroy((Node*)node);
}
static TypeOps ops =
{
    destroy
};
TypeOps* node_ops()
{
    return &ops;
}
