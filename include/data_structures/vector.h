#pragma once

#include <SDL3/SDL_stdinc.h>
#include "helper_functions/error_handling.h"

typedef struct Vector Vector;

Vector* vector_create();
void vector_destroy(Vector* vector);

void vector_add(Vector* vector, void* data);
void* vector_remove_at(Vector* vector, int index);

void vector_set_at(Vector* vector, int index, void* data);

void* vector_get_at(const Vector* vector, int index);
int vector_get_size(const Vector* vector);

void vector_swap(Vector* vector, int index1, int index2);
int vector_search(const Vector* vector, const void* data);

void verify_vector(const Vector* vector);
void verify_vector_index(const Vector* vector, int index);
