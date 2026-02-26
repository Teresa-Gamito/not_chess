typedef enum NodeType {
    
} NodeType;

typedef struct Node {
    NodeType type;
    char *name;
    Node **node; // array of pointers to other nodes
} Node;
