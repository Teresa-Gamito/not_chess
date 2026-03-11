#include "../../../include/game/tree/tree.h"
#include <stdio.h>
#include <stdlib.h>

struct Tree
{
    Node *nodes;
};

Tree *create_tree()
{
    Tree *tree = malloc(sizeof(Tree));
    if (!tree)
    {
        perror("Could not initialize tree: malloc");
        exit(1);
    }

    return tree;
}

void destroy_tree(Tree *tree)
{
    // TODO
    free(tree);
}
