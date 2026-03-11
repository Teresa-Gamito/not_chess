#include <stdio.h>
#include <stdlib.h>

typedef enum NodeType {
    test,
    // TODO define types 
} NodeType;

typedef struct Node Node; 

//create
Node *node_create(NodeType type);
void node_destroy(Node *node);

// set
void node_set_type(Node *node, NodeType node_type);
void node_set_locked(Node *node);
void node_set_unlocked(Node *node);
void node_set_purchased(Node *node);
void node_set_unpurchased(Node *node);
void node_set_name(Node *node, char *name);
void node_set_description(Node *node, char *description);

void node_add_next(Node *node, Node *next_node);
void node_add_previous(Node *node, Node *prev_node);

// get
int node_is_locked(Node node);
int node_is_purchased(Node node);
NodeType node_get_type(Node node);

