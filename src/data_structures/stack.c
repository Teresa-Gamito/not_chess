#include "include/data_structures/stack.h"

Stack* stack_create()
{
    return vector_create();
}

void stack_destroy(Stack* stack)
{
    verify_stack(stack);

    vector_destroy(stack);
}

void stack_push(Stack* stack, void* data)
{
    verify_stack(stack);

    vector_add(stack, data);
}

void* stack_pop(Stack* stack)
{
    verify_stack(stack);

    return vector_remove_at(stack, vector_get_size(stack) - 1);
}

void* stack_peek(const Stack* stack)
{
    verify_stack(stack);

    return vector_get_at(stack, stack_get_size(stack) - 1);
}

int stack_get_size(const Stack* stack)
{
    verify_stack(stack);

    return vector_get_size(stack);
}

void verify_stack(const Stack* stack)
{
    verify(stack == NULL, "Stack does not exist");
}
