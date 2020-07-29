#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "vector.h"

// size will extend
struct vector {
    char *data;
    int   sizeof_type;   // sizeof(type), char -> 1, int -> 4
    int   size;          // total size = sizeof_type * size
    int   capacity;      // if size > capacity? realloc
};

// create(C)
struct vector *vector_alloc(int sizeof_type, int size)
{
    struct vector *vec = (struct vector *)malloc(sizeof(struct vector));
    if (!vec) {
        goto vec_alloc_err;
    }
    vec->size = 0;
    vec->sizeof_type = sizeof_type;
    vec->capacity =  size + 1;
    vec->data = (char *)malloc(vec->capacity * vec->sizeof_type);
    if (!vec->data) {
        goto data_alloc_err;
    }

    return vec;

data_alloc_err:
    free(vec);
vec_alloc_err:
    fprintf(stderr, "alloc error \n");
    return NULL;
}

// delete(D)
void vector_free(struct vector *vec)
{
    if (vec && vec->data) {
        free(vec->data);
        vec->data = NULL;
        vec->capacity = 0;
        vec->size = 0;
        vec->sizeof_type = 0;
    }
    if (vec) {
        free(vec);
    }
}

// insert(U)
static int vector_check_or_realloc__(struct vector *vec) 
{
    assert(vec && vec->data);
    if (vec->size < vec->capacity) {
        return 1;
    }
    // space is not enought
    int new_capacity = vec->capacity * 2;
    char *new_data = (char *)realloc(vec->data, new_capacity * vec->sizeof_type);
    if (!new_data) {
        return 0;
    }
    vec->data = new_data;
    vec->capacity = new_capacity;
    return 1;
}

int vector_push_at(struct vector *vec, void *data, int index)
{
    if (vector_check_or_realloc__(vec) == 0) {
        return 0;
    }
    if (index < vec->size) {
        memmove(vec->data + vec->sizeof_type * (index + 1), 
            vec->data + vec->sizeof_type * index, 
            vec->sizeof_type * (vec->size - index));
    }
    memcpy(vec->data + index * vec->sizeof_type, data, vec->sizeof_type);
    vec->size++;
    return 1;
}

int vector_push_back(struct vector *vec, void *data)
{
    return vector_push_at(vec, data, vec->size);
}

int vector_push_front(struct vector *vec, void *data)
{
    return vector_push_at(vec, data, 0);
}

// remove(D)
int vector_pop_at(struct vector *vec, int index)
{
    if (vec->size == 0) {
        return 0;
    }
    if (index + 1 > vec->size) {
        fprintf(stderr, "out of range\n");
        return 0;
    }
    if (vec->size == index + 1) {
        vec->size--;
        return 1;
    }
    if (vec->size == 1 && index == 0) {
        vec->size = 0;
        return 1;
    }
    memmove(vec->data + vec->sizeof_type * index, 
        vec->data + vec->sizeof_type * (index + 1), 
        vec->sizeof_type * (vec->size - index));
    vec->size--;
    return 1;
}

int vector_pop_back(struct vector *vec)
{
    return vector_pop_at(vec, vec->size - 1);
}

int vector_pop_front(struct vector *vec)
{
    return vector_pop_at(vec, 0);
}

// query(R)
void *vector_begin(struct vector *vec)
{
    return vec->data;
}

void *vector_end(struct vector *vec)
{
    return vec->data + vec->sizeof_type * vec->size;
}

void *vector_at(struct vector *vec, int index)
{
    if (index > vec->size) {
        fprintf(stderr, "out of range\n");
        return NULL;
    }
    return vec->data + vec->sizeof_type * index;
}

void vector_foreach(struct vector *vec, int(*cb)(void *, void *), void *arg)
{
    for (int i = 0; i < vec->size; i++) {
        if (cb) {
            cb(vec->data + i * vec->sizeof_type, arg);
        }
    }
}

int vector_clear(struct vector *vec)
{
    vec->size = 0;
    return 1;
}

int vector_empty(struct vector *vec)
{
    return vec->size == 0 ? 1 : 0;
}

int vector_size(struct vector *vec)
{
    return vec->size;
}