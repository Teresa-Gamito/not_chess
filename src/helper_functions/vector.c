#include "include/helper_functions/vector.h"
#include "helper_functions/error_handling.h"
#include <SDL3/SDL_log.h>
#include <strings.h>

struct Vector
{
    void** data;
    int count;
    TypeOps* ops;
};

Vector* vector_create(TypeOps* ops)
{
    Vector* vector = SDL_malloc(sizeof(Vector));
    verify(vector == NULL, "Vector could not be created");

    vector->data = NULL;
    vector->count = 0;
    vector->ops = ops;

    return vector;
}

void vector_destroy(Vector* vector)
{
    verify_vector(vector);

    vector_destroy_content(vector);
    SDL_free(vector);
}
void vector_destroy_content(Vector* vector)
{
    verify_vector(vector);

    for (int i = 0; i < vector->count; i++)
    {
        if (vector->data[i] == NULL)
        {
            continue;
        }
        vector->ops->destroy(vector->data[i]);
    }
    SDL_free(vector->data);
    vector->data = NULL;
    vector->count = 0;
}
void vector_destroy_item(Vector* vector, void* item)
{
    verify_vector(vector);

    int index = vector_item_get_index(vector, item);
    if (index == -1) return;

    vector->ops->destroy(vector->data[index]);

    for (int i = index; i < vector->count - 1; i++)
    {
        vector->data[i] = vector->data[i + 1];
    }
    vector->count--;
    size_t new_size = sizeof(void*) * vector->count;
    vector->data = SDL_realloc(vector->data, new_size);
    verify(vector->data == NULL, "Could not destroy Vector item: realloc");
}

void vector_add(Vector* vector, void* data)
{
    verify_vector(vector);

    vector->count++;
    size_t new_size = vector->count * sizeof(void*);
    vector->data = SDL_realloc(vector->data, new_size);
    verify(vector == NULL, "Could not add item to Vector: realloc");
    vector->data[vector->count - 1] = data;
}
void vector_set_at(Vector* vector, void* data, int index)
{
    verify_vector(vector);
    verify_vector_index(vector, index);

    vector->data[index] = data;
}
void vector_swap(Vector* vector, int index1, int index2)
{
    void* tmp = vector->data[index1];
    vector->data[index1] = vector->data[index2];
    vector->data[index2] = tmp;
}

int vector_get_count(const Vector* vector)
{
    verify_vector(vector);

    return vector->count;
}
void* vector_get_at(const Vector* vector, int index)
{
    verify_vector(vector);
    verify_vector_index(vector, index);

    return vector->data[index];
}
int vector_item_get_index(const Vector* vector, const void* item)
{
    verify_vector(vector);

    for (int i = 0; i < vector->count; i++)
    {
        if (item == vector->data[i])
        {
            return i;
        }
    }
    verify(true, "Vector does not contain item");
    return -1;
}
TypeOps* vector_get_item_ops(const Vector* vector)
{
    verify_vector(vector);

    return vector->ops;
}



void verify_vector(const Vector* vector)
{
    verify(vector == NULL, "Vector does not exist");
}
void verify_vector_index(const Vector* vector, int index)
{
    verify(index < 0 || index >= vector->count, "Invalid Vector index");
}
