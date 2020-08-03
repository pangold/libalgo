#ifndef __ARRAY_H__
#define __ARRAY_H__

struct array;

struct array *array_alloc(size_t sizeof_type);
void array_free(struct array *array);
void array_clear(struct array *array);
int  array_size(struct array *array);
void array_reverse(struct array *array);
void array_sort(struct array *array, int(*compare)(void *, void *));

void array_push_back(struct array *array, void *data);
void array_push_front(struct array *array, void *data);
void array_push_at(struct array *array, void *data, int n);
void array_push(struct array *array, void *data, int(*condition)(void *, void *), void *arg);

void array_pop_back(struct array *array);
void array_pop_front(struct array *array);
void array_pop_at(struct array *array, int n);
void array_pop(struct array *array, int(*condition)(void *, void *), void *arg);

void *array_begin(struct array *array);
void *array_at(struct array *array, int n);
void *array_next(struct array *array);
void *array_end(struct array *array);
void  array_foreach(struct array *array, int(*cb)(void *, void *), void *arg);

#endif