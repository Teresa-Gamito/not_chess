#include "node.h"

typedef struct Tree {
    Node *nodes;
} Tree;

Tree create_tree();
void destroy_tree(Tree *tree);


