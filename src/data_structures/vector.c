#include "include/data_structures/vector.h"

struct Vector
{
    void** data;
    int size;
};

Vector* vector_create()
{
    Vector* vector = SDL_malloc(sizeof(Vector));
    vector->size = 0;
    vector->data = NULL;
    return vector;
}

void vector_destroy(Vector* vector)
{
    verify_vector(vector);

    SDL_free(vector->data);
    SDL_free(vector);
}

void vector_add(Vector* vector, void* data)
{
    verify_vector(vector);

    vector->size++;
    size_t new_size = vector->size * sizeof(void*);
    vector->data = SDL_realloc(vector->data, new_size);
    vector->data[vector->size - 1] = data;
}

int vector_get_size(const Vector* vector)
{
    verify_vector(vector);

    return vector->size;
}

void* vector_get_at(const Vector* vector, int index)
{
    verify_vector(vector);
    verify_vector_index(vector, index);

    return vector->data[index];
}

void vector_set_at(Vector* vector, int index, void* data)
{
    verify_vector(vector);
    verify_vector_index(vector, index);

    vector->data[index] = data;
}

void* vector_remove_at(Vector* vector, int index)
{
    verify_vector(vector);
    verify_vector_index(vector, index);

    void* data = vector_get_at(vector, index);
    for (int i = index; i < vector->size - 1; i++)
    {
        vector_swap(vector, i, i + 1);
    }
    vector->size--;
    size_t new_size = vector->size * sizeof(void*);
    vector->data = SDL_realloc(vector->data, new_size);
    return data;
}

void vector_swap(Vector* vector, int index1, int index2)
{
    verify_vector(vector);
    verify_vector_index(vector, index1);
    verify_vector_index(vector, index2);

    void* tmp = vector->data[index1];
    vector->data[index1] = vector->data[index2];
    vector->data[index2] = tmp;
}

int vector_search(const Vector* vector, const void* data)
{
    verify_vector(vector);

    for (int i = 0; i < vector->size; i++)
    {
        if (vector->data[i] == data)
        {
            return i;
        }
    }
    return -1;
}

void verify_vector(const Vector* vector)
{
    verify(vector == NULL, "Vector does not exist");
}

void verify_vector_index(const Vector* vector, int index)
{
    verify(index < 0 || index >= vector->size, "Invalid Vector index");
}
