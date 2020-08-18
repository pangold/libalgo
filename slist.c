#include <stdio.h>
#include <assert.h>

#include "slist.h"

typedef struct slist_node {
    void *data;
    struct slist_node *next;
} slist_node_t;

typedef struct slist {
    slist_node_t *head;
    slist_node_t *tail;
    void (*free)(void *);
    size_t size;

} slist_t;

inline static slist_node_t *__slist_node_alloc(void *data)
{
    slist_node_t *node;
    node = (slist_node_t *)malloc(sizeof(slist_node_t));
    if (!node) return NULL;
    node->data = data;
    node->next = NULL;
    return node;
}

inline static void __slist_node_free(slist_node_t *node, void (*free)(void *))
{
    assert(node);
    if (node->data) free(node->data);
    free(node);
}

inline static void __slist_node_clear(slist_node_t *head, void (*free)(void *))
{
    assert(head);
    slist_node_t *next, *node = head;
    for ( ; node; node = next) {
        next = node->next;
        if (node->data) free(node->data);
        free(node);
    }
}

inline static slist_node_t *__slist_node_at(slist_node_t *head, int n)
{
    assert(head);
    slist_node_t *node;
    for (node = head; node->next || n == 0; node = node->next, n--);
    return node;
}

/**
 * @param head head of list
 * @return new head of list
 */
inline static slist_node_t *__slist_reverse(slist_node_t *head)
{
    assert(head);
    if (!head->next) return head;
    slist_node_t *node = head->next;
    slist_node_t *pre = head;
    slist_node_t *next = NULL;
    pre->next = NULL;
    for ( ; node->next; node = next) {
        next = node->next;
        node->next = pre;
        pre = node;
    }
    node->next = pre;
    return node;
}


slist_t *slist_alloc(void (*free)(void *))
{
    slist_t *list;
    list = (slist_t *)malloc(sizeof(slist_t));
    if (!list) return NULL;
    list->head = NULL;
    list->tail = NULL;
    list->free = free;
    list->size = 0;
    return list;
}

void slist_clear(slist_t *list)
{
    if (!list || !list->head) return;
    __slist_node_clear(list->head, list->free);
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void slist_free(slist_t *list)
{
    slist_clear(list);
    free(list);
}

size_t slist_size(slist_t *list)
{
    return list->size;
}

void slist_reverse(slist_t *list)
{
    slist_node_t *node = __slist_reverse(list->head);
    list->tail = list->head;
    list->head = node;
}

void slist_sort(slist_t *list, int(*compare)(void *, void *));

void slist_push_back(slist_t *list, void *data)
{
    slist_node_t *node;
    node = __slist_node_alloc(data);
    if (!list->head) list->head = node;
    if (list->tail) list->tail->next = node;
    list->tail = node;
    list->size++;
}

void slist_push_front(slist_t *list, void *data)
{
    slist_node_t *node;
    node = __slist_node_alloc(data);
    if (!list->tail) list->tail = node;
    if (list->head) node->next = list->head;
    list->head = node;
    list->size++;
}

void slist_push_at(slist_t *list, void *data, int n)
{
    slist_node_t *pos, *node;
    if (!list->head || n == 0) {
        slist_push_front(list, data);
        return;
    }
    pos = __slist_node_at(list->head, n);
    if (list->tail == pos) {
        slist_push_back(list, data);
        return;
    }
    node = __slist_node_alloc(data);
    node->next = pos->next;
    pos->next = node;
    list->size++;
}

// not recommented
void *slist_pop_back(slist_t *list)
{
    void *data;
    slist_node_t *node, *next;
    if (!list->head) return NULL;
    node = list->head;
    next = node->next;
    for ( ; next && next->next; next = next->next, node = node->next);
    if (!next) list->head = NULL;
    list->tail = node;
    list->size--;
    node->next = NULL;
    data = next->data;
    free(next);
    return data;
}

void *slist_pop_front(slist_t *list)
{
    void *data;
    slist_node_t *node;
    if (!list->head) return NULL;
    node = list->head;
    list->head = node->next;
    list->size--;
    if (list->head == NULL) {
        list->tail = NULL;
    }
    data = node->data;
    free(node);
    return data;
}

// not recommented(low efficient)
void *slist_pop_at(slist_t *list, int n)
{
    void *data;
    slist_node_t *node, *next;
    if (!list->head) return NULL;
    node = list->head;
    next = node->next;
    for ( ; next && next->next && n > 1; next = next->next, node = node->next, n--);
    if (!next) list->head = NULL;
    list->tail = node;
    node->next = NULL;
    data = next->data;
    free(next);
    return data;
}

void *slist_front(slist_t *list)
{
    if (!list || !list->head) return NULL;
    return list->head->data;
}

void *slist_back(slist_t *list)
{
    if (!list || !list->tail) return NULL;
    return list->tail->data;
}