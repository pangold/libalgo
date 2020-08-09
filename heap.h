#ifndef __HEAP_H__
#define __HEAP_H__

typedef struct heap heap_t;

heap_t *heap_alloc(size_t sizeof_type,int(*compare)(void *, void *));
void heap_free(heap_t *heap);
void heap_clear(heap_t *heap);
size_t heap_size(heap_t *heap);

void heap_push(heap_t *heap, void *data);
void heap_pop(heap_t *heap);
void *heap_top(heap_t *heap);

#endif