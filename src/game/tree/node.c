#include "../../../include/game/tree/node.h"
#include <stdio.h>
#include <stdlib.h>

struct Node {
    // array of pointers to nodes
    // nodes that require this node
    Node **next_nodes;
    int num_next_nodes;
    // nodes required to unlock this node
    Node **prev_nodes;
    int num_prev_nodes;

    // data
    // id
    NodeType type;
    // verifies if the node is locked 
    int is_locked;
    // verifies if the node was purchased
    int is_purchased;
    // cost to buy this node
    int cost;
    // name of the node
    char *name;
    // description of the node
    char *description;
    // which player bought this node
    // Player player
};

Node *node_create(NodeType type)
{
    Node *node = malloc(sizeof(Node));
    if (!node)
    {
        perror("Could not create node: malloc");
        exit(1);
    }
    return node;
}

void node_destroy(Node *node)
{
    free(node->name);
    free(node->description);
    free(node->next_nodes);
    free(node->prev_nodes);
    free(node);
}

void node_set_type(Node *node, NodeType node_type)
{
    node->type = node_type;
}

void node_set_locked(Node *node)
{
    node->is_locked = 1;
}

void node_set_unlocked(Node *node)
{
    node->is_locked = 0;
}

void node_set_purchased(Node *node)
{
    node->is_purchased = 1;
}

void node_set_unpurchased(Node *node)
{
    node->is_purchased = 0;
}

void node_set_name(Node *node, char *name)
{
    node->name = name;
}

void node_set_description(Node *node, char *description)
{
    node->description = description;
}

void node_add_next(Node *node, Node *next_node)
{
    (node->num_next_nodes)++;
    node->next_nodes = (Node**) realloc(node->next_nodes, sizeof(Node*) * node->num_next_nodes);
    if (!node->next_nodes)
    {
        perror("Could not add node: realloc");
        exit(1);
    }
    node->next_nodes[node->num_next_nodes] = next_node;
}

void node_add_previous(Node *node, Node *prev_node)
{
    (node->num_prev_nodes)++;
    node->prev_nodes = (Node**) realloc(node->prev_nodes, sizeof(Node*) * node->num_prev_nodes);
    if (!node->prev_nodes)
    {
        perror("Could not add node: realloc");
        exit(1);
    }
    node->prev_nodes[node->num_prev_nodes] = prev_node;
}

int node_is_locked(Node node)
{
    return node.is_locked;
}

int node_is_purchased(Node node)
{
    return node.is_purchased;
}

NodeType node_get_type(Node node)
{
    return node.type;
}
