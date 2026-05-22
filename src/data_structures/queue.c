#include "data_structures/queue.h"

Queue* queue_create()
{
    return vector_create();
}

void queue_destroy(Queue* queue)
{
    verify_queue(queue);

    vector_destroy(queue);
}

void queue_push(Queue* queue, void* data)
{
    verify_queue(queue);

    vector_add(queue, data);
}

void* queue_pop(Queue* queue)
{
    verify_queue(queue);

    if (queue_get_size(queue) <= 0)
    {
        return NULL;
    }
    return vector_remove_at(queue, 0);
}

int queue_get_size(const Queue* queue)
{
    verify_queue(queue);

    return vector_get_size(queue);
}

void verify_queue(const Queue* queue)
{
    verify(queue == NULL, "Queue does not exist");
}
