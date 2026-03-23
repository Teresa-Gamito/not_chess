#include "../../../include/game/tree/tree.h"

struct Tree
{
    Node **nodes;
    int count;
};

Tree *create_tree()
{
    Tree *tree = malloc(sizeof(Tree));
    if (!tree)
    {
        // TODO:
    }

    return tree;
}

void destroy_tree(Tree *tree)
{
    // TODO:
    free(tree);
}
