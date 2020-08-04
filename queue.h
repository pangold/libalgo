#ifndef __QUEUE_H__
#define __QUEUE_H__

typedef struct list queue_t;

queue_t *queue_alloc();
void queue_free(queue_t *queue, void(*freecb)(void *));
void queue_clear(queue_t *queue, void(*freecb)(void *));
int  queue_size(queue_t *queue);

void  queue_push(queue_t *queue, void *data);
void *queue_pop(queue_t *queue);
void *queue_top(queue_t *queue);

#endif