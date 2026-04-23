#include "include/helper_functions/vector.h"
#include "helper_functions/error_handling.h"

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
void vector_add(Vector* vector, void* data)
{
    verify(vector == NULL, "Vector does not exist");
    verify(data == NULL, "Could not add data to vector: NULL");

    vector->count++;
    size_t new_size = vector->count * sizeof(void*);
    vector->data = SDL_realloc(vector->data, new_size);
    verify(vector == NULL, "Could not add item to Vector: realloc");
    vector->data[vector->count - 1] = data;
}

void vector_destroy(Vector* vector)
{
    verify(vector == NULL, "Vector does not exist");

    vector_destroy_content(vector);
    SDL_free(vector);
}
void vector_destroy_content(Vector* vector)
{
    verify(vector == NULL, "Vector does not exist");

    for (int i = 0; i < vector->count; i++)
    {
        vector->ops->destroy(vector->data[i]);
    }
    SDL_free(vector->data);
    vector->data = NULL;
    vector->count = 0;
}
void vector_destroy_item(Vector* vector, void* item)
{
    verify(vector == NULL, "Vector does not exist");

    int index = -1;
    for (int i = 0; i < vector_get_count(vector); i++)
    {
        if (item == vector->data[i])
        {
            index = i;
            break;
        }
    }

    vector->ops->destroy(vector->data[index]);

    for (int i = index; i < vector->count - 2; i++)
    {
        vector->data[i] = vector->data[i + 1];
    }
    // SDL_memmove(
    //     vector->data[index],
    //     vector->data[index + 1],
    //     (vector->count - index - 1) * sizeof(void*)
    // );
    vector->count--;
    size_t new_size = sizeof(void*) * vector->count;
    vector->data = SDL_realloc(vector->data, new_size);
    verify(vector->data == NULL, "Could not destroy Vector item: realloc");
}

int vector_get_count(const Vector* vector)
{
    verify(vector == NULL, "Vector does not exist");

    return vector->count;
}
void* vector_get_at(Vector* vector, int index)
{
    verify(vector == NULL, "Vector does not exist");
    verify(index < 0 && index >= vector->count, "Invalid index");

    return vector->data[index];
}
TypeOps* vector_get_item_ops(Vector* vector)
{
    verify(vector == NULL, "Vector does not exist");

    return vector->ops;
}
