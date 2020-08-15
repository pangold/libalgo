#ifndef __SORTED_SET_H__
#define __SORTED_SET_H__

typedef struct rb_tree sorted_set_t;
typedef void (*sorted_set_free_t)(void *);
typedef int (*sorted_set_compare_t)(void *, void *);

sorted_set_t *sorted_set_alloc(sorted_set_compare_t compare);
void sorted_set_free(sorted_set_t *set, sorted_set_free_t cb);
void sorted_set_clear(sorted_set_t *set, sorted_set_free_t cb);
size_t sorted_set_size(sorted_set_t *set);

void sorted_set_push(sorted_set_t *set, void *data);
void *sorted_set_pop(sorted_set_t *set, void *data);
void *sorted_set_pop_front(sorted_set_t *set);
void *sorted_set_pop_back(sorted_set_t *set);

void *sorted_set_front(sorted_set_t *set);
void *sorted_set_back(sorted_set_t *set);

void sorted_set_foreach(sorted_set_t *set, void(*cb)(void *, void *), void *arg);

#endif