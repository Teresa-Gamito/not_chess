// resizable array
#pragma once

#include <SDL3/SDL_stdinc.h>
#include "include/helper_functions/typeops.h"
#include "include/helper_functions/error_handling.h"

typedef struct Vector Vector;

Vector* vector_create(TypeOps* ops);
void vector_add(Vector* vector, void* data);

void vector_destroy(Vector* vector);
void vector_destroy_content(Vector* vector);
void vector_destroy_item(Vector* vector, void* item);

int vector_get_count(const Vector* vector);
void* vector_get_at(const Vector* vector, int index);
TypeOps* vector_get_item_ops(const Vector* vector);
