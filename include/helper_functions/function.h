#pragma once

#include <SDL3/SDL_stdinc.h>
#include "helper_functions/error_handling.h"
#include "helper_functions/typeops.h"

typedef struct Function Function;

Function* function_create(void (*fn)(void* arg1, void* arg2), void* arg1, void* arg2);
void function_set(Function* function, void (*fn)(void* arg1, void* arg2), void* arg1, void* arg2);
void function_destroy(Function* function);
void function_execute(Function* function);

TypeOps* function_ops();
