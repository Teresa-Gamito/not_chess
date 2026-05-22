#pragma once

#include "data_structures/vector.h"
#include "helper_functions/error_handling.h"

typedef Vector Queue;

Queue* queue_create();
void queue_destroy(Queue* queue);

void queue_push(Queue* queue, void* data);
void* queue_pop(Queue* queue);

int queue_get_size(const Queue* queue);

void verify_queue(const Queue* queue);
