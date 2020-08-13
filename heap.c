#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "heap.h"
#include "array.h"

typedef struct heap {
    array_t *data;
    int    (*compare)(void *, void *);
} heap_t;

static heap_t *__heap_alloc(int sizeof_type, int(*compare)(void *, void *))
{
    heap_t * heap = (heap_t *)malloc(sizeof(heap_t));
    if (!heap) return NULL;
    heap->data = array_alloc(sizeof_type);
    heap->compare = compare;
    return heap;
}

extern void __array_swap(array_t *array, size_t m, size_t n);

static void __heap_down(heap_t *heap, size_t root)
{
    size_t parent = root;
    size_t child = parent * 2 + 1;
    size_t size = array_size(heap->data);
    void *data, *data1, *data2;
    while (child < size) {
        data1 = array_at(heap->data, child);
        data2 = array_at(heap->data, child + 1);
        if (child + 1 < size && heap->compare(data2, data1)) child++;
        data = array_at(heap->data, parent);
        data1 = array_at(heap->data, child);
        if (!heap->compare(data1, data)) break;
        __array_swap(heap->data, child, parent);
        parent = child;
        child = parent * 2 + 1;
    }
}
static void __heap_up(heap_t *heap, size_t child)
{
    size_t parent = (child - 1) / 2;
    while (parent >= 0) {
        void *data  = array_at(heap->data, parent);
        void *data1 = array_at(heap->data, child);
        if (!heap->compare(data1, data)) break;
        __array_swap(heap->data, child, parent);
        child = parent;
        parent = (child - 1) / 2;
    }
}

heap_t *heap_alloc(size_t sizeof_type, int(*compare)(void *, void *))
{
    return __heap_alloc(sizeof_type, compare);
}

void heap_free(heap_t *heap)
{
    array_free(heap->data);
    free(heap);
}

void heap_clear(heap_t *heap)
{
    array_clear(heap->data);
}

size_t heap_size(heap_t *heap)
{
    return array_size(heap->data);
}

void heap_push(heap_t *heap, void *data)
{
    array_push_back(heap->data, data);
    __heap_up(heap, array_size(heap->data) - 1);
}

void heap_pop(heap_t *heap)
{
    __array_swap(heap->data, 0, array_size(heap->data) - 1);
    array_pop_back(heap->data);
    __heap_down(heap, 0);
}

void *heap_top(heap_t *heap)
{
    return array_data(heap->data);
}

#if defined(ENABLE_HEAP_TEST)
int heap_int_compare_less(void *i1, void *i2)
{
    return *(int *)i1 < *(int *)i2;
}
int heap_int_compare_more(void *i1, void *i2)
{
    return *(int *)i1 > *(int *)i2;
}
void heap_int_test()
{
    heap_t *heap = heap_alloc(sizeof(int), heap_int_compare_less);
    int int_array[] = { 2,5,6,4,3,1,8,9,7,0 };
    heap_push(heap, &int_array[0]);
    heap_push(heap, &int_array[1]);
    heap_push(heap, &int_array[2]);
    heap_push(heap, &int_array[3]);
    heap_push(heap, &int_array[4]);
    heap_push(heap, &int_array[5]);
    heap_push(heap, &int_array[6]);
    heap_push(heap, &int_array[7]);
    heap_push(heap, &int_array[8]);
    heap_push(heap, &int_array[9]);
    printf("top: %d\n", *(int *)heap_top(heap));
    assert(*(int *)heap_top(heap) == 0);
    heap_pop(heap);
    printf("top: %d\n", *(int *)heap_top(heap));
    assert(*(int *)heap_top(heap) == 1);
    heap_pop(heap);
    printf("top: %d\n", *(int *)heap_top(heap));
    assert(*(int *)heap_top(heap) == 2);
    heap_pop(heap);
    printf("top: %d\n", *(int *)heap_top(heap));
    assert(*(int *)heap_top(heap) == 3);
    heap_pop(heap);
    printf("top: %d\n", *(int *)heap_top(heap));
    assert(*(int *)heap_top(heap) == 4);
    heap_pop(heap);
    printf("top: %d\n", *(int *)heap_top(heap));
    assert(*(int *)heap_top(heap) == 5);
    heap_pop(heap);
    printf("top: %d\n", *(int *)heap_top(heap));
    assert(*(int *)heap_top(heap) == 6);
    heap_pop(heap);
    printf("top: %d\n", *(int *)heap_top(heap));
    assert(*(int *)heap_top(heap) == 7);
    heap_pop(heap);
    printf("top: %d\n", *(int *)heap_top(heap));
    assert(*(int *)heap_top(heap) == 8);
    heap_pop(heap);
    printf("top: %d\n", *(int *)heap_top(heap));
    assert(*(int *)heap_top(heap) == 9);
    heap_free(heap);
}
#endif