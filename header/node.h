#include <stdbool.h>

typedef enum NodeType {
    
} NodeType;

typedef struct Node {
    NodeType type; // id
    Node **next_nodes; // array of pointers to other nodes
    bool is_locked; // verifies if the node is unlocked
    bool is_purchased;
} Node;

Node create_node(NodeType type, Node **next_nodes, bool is_locked, bool is_purchased);
void destroy_node(Node *node);


