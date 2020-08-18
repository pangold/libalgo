#ifndef __SLIST_H__
#define __SLIST_H__

typedef struct slist slist_t;

slist_t *slist_alloc(void (*free)(void *));
void slist_free(slist_t *list);
void slist_clear(slist_t *list);
size_t slist_size(slist_t *list);
void slist_reverse(slist_t *list);
void slist_sort(slist_t *list, int(*compare)(void *, void *));

void slist_push_back(slist_t *list, void *data);
void slist_push_front(slist_t *list, void *data);
void slist_push_at(slist_t *list, void *data, int n);

void *slist_pop_back(slist_t *list);
void *slist_pop_front(slist_t *list);
void *slist_pop_at(slist_t *list, int n);

void *slist_front(slist_t *list);
void *slist_back(slist_t *list);

void *slist_begin(slist_t *list);
void *slist_at(slist_t *list, int n);
void *slist_next(slist_t *list);
void *slist_end(slist_t *list);
void  slist_foreach(slist_t *list, int(*cb)(void *, void *), void *arg);

#endif