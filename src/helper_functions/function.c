#include "include/helper_functions/function.h"

struct Function 
{
    void (*fn)(void* arg1, void* arg2);
    void* arg1;
    void* arg2;
};

Function* function_create(void (*fn)(void* arg1, void* arg2), void* arg1, void* arg2)
{
    Function* function = SDL_malloc(sizeof(Function));
    function_set(function, fn, arg1, arg2);
    return function;
}
void function_set(Function* function, void (*fn)(void* arg1, void* arg2), void* arg1, void* arg2)
{
    verify(function == NULL, "Function does not exist");
    function->fn = fn;
    function->arg1 = arg1;
    function->arg2 = arg2;
}

void function_destroy(Function* function)
{
    verify(function == NULL, "Function does not exist");
    SDL_free(function);
}

void function_execute(Function* function)
{
    if (function == NULL)
    {
        return;
    }
    if (function->fn == NULL) 
    {
        return;
    }
    function->fn(function->arg1, function->arg2);
}

static void destroy(void* func)
{
    function_destroy(func);
}
static TypeOps ops = 
    {
        destroy
    };
TypeOps* function_ops()
{
    return &ops;
}
