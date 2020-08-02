#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "dynamic_array.h"

#ifndef NULL
#define NULL (void *)0
#endif

#define DYNAMIC_ARRAY_INIT_LEN 10

struct dynamic_array {
    char  *data;
    size_t sizeof_type;   // sizeof(type), char -> 1, int -> 4
    size_t size;          // total size = sizeof_type * size
    size_t capacity;      // if size > capacity? realloc
};

inline static struct dynamic_array *__array_alloc(size_t sizeof_type)
{
    struct dynamic_array *array;
    array = (struct dynamic_array *)malloc(sizeof(struct dynamic_array));
    if (!array) return NULL;
    array->sizeof_type = sizeof_type;
    array->size = sizeof_type * DYNAMIC_ARRAY_INIT_LEN;
    array->capacity = array->size;
    array->data = (char *)malloc(array->capacity);
    return array;
}

inline static void __array_free(struct dynamic_array *array)
{
    if (array->data) free(array->data);
    free(array);
}

inline static void __array_clear(struct dynamic_array *array)
{
    array->size = 0;
}

inline static void __array_realloc(struct dynamic_array *array, size_t size)
{
    assert(size > array->size);
    if (array->data) array->data = realloc(array->data, size);
}

inline static void __array_push_back(struct dynamic_array *array, void *data)
{
    if (array->size == array->capacity)
        __array_realloc(array, array->capacity * 2);
    memcpy(array->data + array->size, data, array->sizeof_type);
    array->size += array->sizeof_type;
}

inline static void __array_pop_back(struct dynamic_array *array)
{
    if (array->size == 0) return;
    array->size -= array->sizeof_type;
}

struct dynamic_array *dynamic_array_alloc(size_t sizeof_type);
void dynamic_array_free(struct dynamic_array *list);
void dynamic_array_clear(struct dynamic_array *list);
int  dynamic_array_size(struct dynamic_array *list);
void dynamic_array_reverse(struct dynamic_array *list);
void dynamic_array_sort(struct dynamic_array *list, int(*compare)(void *, void *));

void dynamic_array_push_back(struct dynamic_array *list, void *data);
void dynamic_array_push_front(struct dynamic_array *list, void *data);
void dynamic_array_push_at(struct dynamic_array *list, void *data, int n);
void dynamic_array_push(struct dynamic_array *list, void *data, int(*condition)(void *, void *), void *arg);

void *dynamic_array_pop_back(struct dynamic_array *list);
void *dynamic_array_pop_front(struct dynamic_array *list);
void *dynamic_array_pop_at(struct dynamic_array *list, int n);
void *dynamic_array_pop(struct dynamic_array *list, int(*condition)(void *, void *), void *arg);

void *dynamic_array_begin(struct dynamic_array *list);
void *dynamic_array_at(struct dynamic_array *list, int n);
void *dynamic_array_next(struct dynamic_array *list);
void *dynamic_array_end(struct dynamic_array *list);
void  dynamic_array_foreach(struct dynamic_array *list, int(*cb)(void *, void *), void *arg);