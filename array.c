#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "array.h"

#ifndef NULL
#define NULL (void *)0
#endif
#define ARRAY_INIT_LEN 10

struct array {
    char  *data;
    size_t sizeof_type;   // sizeof(type), char -> 1, int -> 4
    size_t size;          // total size = sizeof_type * size
    size_t capacity;      // if size > capacity? realloc
    size_t current;
};

inline static void __array_realloc(array_t *array, size_t size)
{
    assert(size > array->size);
    if (array->data) array->data = realloc(array->data, size);
}

inline static void __array_move_backward(array_t *array, size_t pos, size_t n) 
{
    assert(n > 0);
    if (array->size + array->sizeof_type * n > array->capacity) 
        __array_realloc(array, array->capacity * 2);
    memmove(array->data + array->sizeof_type * (pos + n), 
        array->data + array->sizeof_type * pos, 
        array->sizeof_type * (array->size - pos));
}

inline static void __array_move_frontward(array_t *array, size_t pos, size_t n)
{
    assert(n * array->sizeof_type <= array->size);
    memmove(array->data + array->sizeof_type * (pos - n), 
        array->data + array->sizeof_type * pos, 
        array->sizeof_type * (array->size - pos));
}

inline static void __array_push_at(array_t *array, const void *data, size_t pos)
{
    __array_move_backward(array, pos, 1);
    memcpy(array->data + array->sizeof_type * pos, data, array->sizeof_type);
    array->size += array->sizeof_type;
}

inline static void __array_push_front(array_t *array, const void *data)
{
    __array_push_at(array, data, 0);
}

inline static void __array_push_back(array_t *array, const void *data)
{
    if (array->size == array->capacity)
        __array_realloc(array, array->capacity * 2);
    memcpy(array->data + array->size, data, array->sizeof_type);
    array->size += array->sizeof_type;
}

inline static void __array_pop_at(array_t *array, void *data, size_t pos)
{
    if (data) memcpy(data, array->data + array->sizeof_type * pos, array->sizeof_type);
    __array_move_frontward(array, pos, 1);
    array->size -= array->sizeof_type;
}

inline static void __array_pop_front(array_t *array, void *data)
{
    __array_pop_at(array, data, array->size - array->sizeof_type);
}

inline static void __array_pop_back(array_t *array, void *data)
{
    if (array->size == 0) return;
    array->size -= array->sizeof_type;
    if (data) memcpy(data, array->size * array->sizeof_type, array->sizeof_type);
}

static void __array_swap(array_t *array, size_t m, size_t n)
{
    char *temp = (char *)malloc(array->sizeof_type);
    memcpy(temp, array->data + array->sizeof_type * m, array->sizeof_type);
    memcpy(array->data + array->sizeof_type * m, array->data + array->sizeof_type * n, array->sizeof_type);
    memcpy(array->data + array->sizeof_type * n, temp, array->sizeof_type);
    free(temp);
}

static void __array_reverse(array_t *array)
{
    size_t len = array->size / array->sizeof_type;
    size_t half = len / 2;
    for (int i = 0; i < half; i++) {
        __array_swap(array, i, len - i - 1);
    }
}

static void __array_bubble_sort(array_t *array, int(*compare)(void *, void *))
{
    
}

static void __array_select_sort(array_t *array, int(*compare)(void *, void *))
{
    
}

static void __array_insert_sort(array_t *array, int(*compare)(void *, void *))
{
    
}

array_t *array_alloc(size_t sizeof_type)
{
    array_t *array;
    array = (array_t *)malloc(sizeof(array_t));
    if (!array) return NULL;
    array->sizeof_type = sizeof_type;
    array->size = sizeof_type * ARRAY_INIT_LEN;
    array->capacity = array->size;
    array->data = (char *)malloc(array->capacity);
    return array;
}

void array_free(array_t *array)
{
    if (array->data) free(array->data);
    free(array);
}

void array_clear(array_t *array)
{
    array->size = 0;
}

int array_size(array_t *array)
{
    return array->size / array->sizeof_type;
}

void array_reverse(array_t *array)
{
    __array_reverse(array);
}

void array_sort(array_t *array, int(*compare)(void *, void *))
{
#if defined(USE_BUBBLE_SORT)
    __array_bubble_sort(array, compare);
#elif defined(USE_SELECT_SORT)
    __array_select_sort(array, compare);
#elif defined(USE_INSERT_SORT)
    __array_insert_sort(array, compare);
#else
    __array_bubble_sort(array, compare);
#endif
}

void array_push_back(array_t *array, const void *data)
{
    __array_push_back(array, data);
}

void array_push_front(array_t *array, const void *data)
{
    __array_push_front(array, data);
}

void array_push_at(array_t *array, const void *data, int n)
{
    __array_push_at(array, data, n);
}

void array_push(array_t *array, void *data, int(*condition)(void *, void *), void *arg)
{
    for (int i = 0; i < array->size; i += array->sizeof_type) {
        if (condition(array->data + i, arg) != 0) {
            __array_push_at(array, data, i / array->sizeof_type);
        }
    }
}

void array_pop_back(array_t *array)
{
    __array_pop_back(array, NULL);
}

void array_pop_front(array_t *array)
{
    __array_pop_front(array, NULL);
}

void array_pop_at(array_t *array, int n)
{
    __array_pop_at(array, NULL, n);
}

void array_pop(array_t *array, int(*condition)(void *, void *), void *arg)
{
    for (int i = 0; i < array->size; i += array->sizeof_type) {
        if (condition(array->data + i, arg) != 0) {
            __array_pop_at(array, NULL, i / array->sizeof_type);
        }
    }
}

void *array_begin(array_t *array)
{
    array->current = 0;
    return array->data;
}

void *array_at(array_t *array, int n)
{
    array->current = n;
    return array->data + array->sizeof_type * n;
}

void *array_next(array_t *array)
{
    array->current++;
    return array->data + array->sizeof_type * array->current;
}

void *array_end(array_t *array)
{
    return array->data + array->sizeof_type * array->size;
}

void array_foreach(array_t *array, int(*cb)(void *, void *), void *arg)
{
    for (int i = 0; i < array->size; i += array->sizeof_type) {
        cb(array->data + i, arg);
    }
}

#if defined(ENABLE_ARRAY_TEST)

#endif
