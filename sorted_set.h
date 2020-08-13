#ifndef __SORTED_SET_H__
#define __SORTED_SET_H__

typedef struct rb_tree sorted_set_t;

sorted_set_t *sorted_set_alloc();
void sorted_set_free(sorted_set_t *set);
void sorted_set_clear(sorted_set_t *set);
int  sorted_set_size(sorted_set_t *set);

void sorted_set_push(sorted_set_t *set, void *value);
void sorted_set_pop(sorted_set_t *set, void *value);

void *sorted_set_front(sorted_set_t *set);
void *sorted_set_back(sorted_set_t *set);

void *sorted_set_begin(sorted_set_t *set);
void *sorted_set_end(sorted_set_t *set);
void *sorted_set_next(sorted_set_t *set);
void  sorted_set_foreach(sorted_set_t *set, void(*cb)(void *, void *, void *), void *arg);

#endif