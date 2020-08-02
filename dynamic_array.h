#ifndef __DYNAMIC_ARRAY_H__
#define __DYNAMIC_ARRAY_H__

struct dynamic_array;

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

#endif