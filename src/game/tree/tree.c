#include "include/game/tree/tree.h"

struct Tree
{
    Vector* nodes;
};

Tree* tree_create()
{
    Tree* tree = SDL_malloc(sizeof(Tree));

    tree->nodes = vector_create(node_ops());
    return tree;
}

void tree_set_default(Tree* tree)
{
    Node* node = node_create(TASK_ADD_PAWN, 1);
    tree_add_node(tree, node);
    node = node_create(TASK_ADD_PAWN, 1);
    tree_add_node(tree, node);
    node = node_create(TASK_ADD_PAWN, 1);
    tree_add_node(tree, node);
    node = node_create(TASK_ADD_PAWN, 1);
    tree_add_node(tree, node);
    node = node_create(TASK_ADD_PAWN, 1);
    tree_add_node(tree, node);
}

void tree_destroy(Tree* tree)
{
    vector_destroy(tree->nodes);
    SDL_free(tree);
}

void tree_add_node(Tree* tree, Node* node)
{
    vector_add(tree->nodes, node);
}

int tree_get_node_count(const Tree* tree)
{
    return vector_get_count(tree->nodes);
}
Node* tree_get_node_at(const Tree* tree, int index)
{
    return (Node*)vector_get_at(tree->nodes, index);
}
