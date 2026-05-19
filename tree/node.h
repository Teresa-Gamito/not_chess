// nodes on the tree
// can be purchased to change the game flow
// TODO: abstract
#pragma once

#include <SDL3/SDL_stdinc.h>
#include "helper_functions/typeops.h"
#include "helper_functions/error_handling.h"
#include "game/board/board.h"
#include "game/ui/board_ui.h"
#include "game/board/player.h"
#include "game/board/task.h"

typedef struct Node Node;

Node* node_create(Task task, int cost);
void node_destroy(Node* node);

bool node_can_purchase(const Node* node, int player_points);
void node_purchase(void* board_ui, void* node_to_purchase);

Task node_get_task(const Node* node);
int node_get_cost(const Node* node);
TypeOps* node_ops();
