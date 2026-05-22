#pragma once

#include <SDL3/SDL_stdinc.h>
#include "vector.h"
#include "helper_functions/error_handling.h"

typedef Vector Stack;

Stack* stack_create();
void stack_destroy(Stack* stack);

void stack_push(Stack* stack, void* data);
void* stack_pop(Stack* stack);

int stack_get_size(const Stack* stack);

void verify_stack(const Stack* stack);
