#pragma once

#include <SDL3/SDL_stdinc.h>
#include "error_handling.h"
#include "typeops.h"

typedef struct Vector Vector;

Vector* vector_create(TypeOps* ops);
void vector_destroy(Vector* vector);

void vector_add(Vector* vector, void* data);
int vector_get_count(const Vector* vector);

void* vector_get_at(Vector* vector, int index);
void vector_destroy_item(Vector* vector, void* item);

TypeOps* vector_get_item_ops(Vector* vector);
