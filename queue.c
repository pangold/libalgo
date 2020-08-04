#include "queue.h"
#include "list.h"

queue_t *queue_alloc()
{
    return list_alloc();
}

void queue_free(queue_t *queue, void(*freecb)(void *))
{
    list_free(queue, freecb);
}

void queue_clear(queue_t *queue, void(*freecb)(void *))
{
    list_clear(queue, freecb);
}

int queue_size(queue_t *queue)
{
    return list_size(queue);
}

void queue_push(queue_t *queue, void *data)
{
    list_push_back(queue, data);
}

void *queue_pop(queue_t *queue)
{
    return list_pop_front(queue);
}

void *queue_top(queue_t *queue)
{
    return list_front(queue);
}