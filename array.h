#ifndef __ARRAY_H__
#define __ARRAY_H__

typedef struct array array_t;

array_t *array_alloc(size_t sizeof_type);
void array_free(array_t *array);
void array_clear(array_t *array);
int  array_size(array_t *array);
void array_reverse(array_t *array);
void array_sort(array_t *array, int(*compare)(void *, void *));

void array_push_back(array_t *array, void *data);
void array_push_front(array_t *array, void *data);
void array_push_at(array_t *array, void *data, int n);

void array_pop_back(array_t *array);
void array_pop_front(array_t *array);
void array_pop_at(array_t *array, int n);

void *array_data(array_t *array);
void *array_at(array_t *array, int n);
void array_foreach(array_t *array, int(*cb)(void *, void *), void *arg);

#endif