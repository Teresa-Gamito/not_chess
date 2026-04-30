// resizable array
#pragma once

#include <SDL3/SDL_stdinc.h>
#include "include/helper_functions/typeops.h"
#include "include/helper_functions/error_handling.h"

typedef struct Vector Vector;

Vector* vector_create(TypeOps* ops);

void vector_destroy(Vector* vector);
void vector_destroy_content(Vector* vector);
void vector_destroy_item(Vector* vector, void* item);

void vector_add(Vector* vector, void* data);
void vector_set_at(Vector* vector, void* data, int index);
void vector_swap(Vector* vector, int index1, int index2);

int vector_get_count(const Vector* vector);
void* vector_get_at(const Vector* vector, int index);
int vector_item_get_index(const Vector* vector, const void* item);
TypeOps* vector_get_item_ops(const Vector* vector);


void verify_vector(const Vector* vector);
void verify_vector_index(const Vector* vector, int index);
