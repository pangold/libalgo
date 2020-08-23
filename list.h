#ifndef __LIST_H__
#define __LIST_H__

typedef struct list list_t;

list_t *list_alloc();
void list_free(list_t *list, void(*freecb)(void *));
void list_clear(list_t *list, void(*freecb)(void *));
int  list_size(list_t *list);
void list_reverse(list_t *list);
void list_sort(list_t *list, int(*compare)(void *, void *));

void list_push_back(list_t *list, void *data);
void list_push_front(list_t *list, void *data);
void list_push_at(list_t *list, void *data, int n);
void list_push(list_t *list, void *data, int(*condition)(void *, void *), void *arg);

void *list_pop_back(list_t *list);
void *list_pop_front(list_t *list);
void *list_pop_at(list_t *list, int n);
void *list_pop(list_t *list, int(*condition)(void *, void *), void *arg);

void *list_front(list_t *list);
void *list_back(list_t *list);

void *list_begin(list_t *list);
void *list_at(list_t *list, int n);
void *list_next(list_t *list);
void *list_end(list_t *list);
void  list_foreach(list_t *list, void(*cb)(void *, void *), void *arg);

#endif