#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

struct linked_list;

struct linked_list *linked_list_alloc();
void linked_list_free(struct linked_list *list);
void linked_list_clear(struct linked_list *list);
int  linked_list_size(struct linked_list *list);
void linked_list_reverse(struct linked_list *list);
void linked_list_sort(struct linked_list *list, int(*compare)(void *, void *));

void linked_list_push_back(struct linked_list *list, void *data);
void linked_list_push_front(struct linked_list *list, void *data);
void linked_list_push_at(struct linked_list *list, void *data, int n);
void linked_list_push(struct linked_list *list, void *data, int(*condition)(void *, void *), void *arg);

void *linked_list_pop_back(struct linked_list *list);
void *linked_list_pop_front(struct linked_list *list);
void *linked_list_pop_at(struct linked_list *list, int n);
void *linked_list_pop(struct linked_list *list, int(*condition)(void *, void *), void *arg);

void *linked_list_begin(struct linked_list *list);
void *linked_list_at(struct linked_list *list, int n);
void *linked_list_next(struct linked_list *list);
void *linked_list_end(struct linked_list *list);
void  linked_list_foreach(struct linked_list *list, int(*cb)(void *, void *), void *arg);

#endif