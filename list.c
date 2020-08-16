#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "list.h"

#ifndef NULL
#define NULL (void *)0
#endif

typedef struct list_node {
    void *data;
    struct list_node *pre;
    struct list_node *next;
} list_node_t;

inline static list_node_t *__list_node_alloc(void *data)
{
    list_node_t *node;
    node = (list_node_t *)malloc(sizeof(list_node_t));
    if (!node) return NULL;
    node->data = data;
    node->pre = NULL;
    node->next = NULL;
    return node;
}

inline static void *__list_node_free(list_node_t *src)
{
    void *data = src->data;
    free(src);
    return data;
}

inline static void __list_node_clear(list_node_t *head, void(*freecb)(void *))
{
    list_node_t *node, *temp = NULL;
    for (node = head; node && node->next; node = temp) {
        if (freecb) freecb(node->data);
        temp = node->next;
        free(node);
    }
}

inline static list_node_t *__list_node_first(list_node_t *src)
{
    list_node_t *node = NULL;
    for (node = src; node && node->pre; node = node->pre);
    return node;
}

inline static list_node_t *__list_node_last(list_node_t *src)
{
    list_node_t *node = NULL;
    for (node = src; node && node->next; node = node->next);
    return node;
}

inline static list_node_t *__list_node_at(list_node_t *src, int n)
{
    int i = 0;
    list_node_t *node = NULL;
    for (node = src; node && node->next && i < n; node = node->next, i++);
    return node;
}

inline static void __list_node_push_after(list_node_t *src, list_node_t *node)
{
    if (!src || !node) return;
    if (src->next) src->next->pre = node;
    node->next = src->next;
    node->pre = src;
    src->next = node;
}

inline static void __list_node_push_before(list_node_t *src, list_node_t *node)
{
    if (!src || !node) return;
    if (src->pre) src->pre->next = node;
    node->pre = src->pre;
    node->next = src;
    src->pre = node;
}

inline static void __list_node_push_back(list_node_t *src, list_node_t *node)
{
    __list_node_push_after(__list_node_last(src), node);
}

inline static void __list_node_push_front(list_node_t *src, list_node_t *node)
{
    __list_node_push_before(__list_node_first(src), node);
}

inline static list_node_t *__list_node_pop_self(list_node_t *src)
{
    if (!src) return NULL;
    if (src->pre) src->pre->next = src->next;
    if (src->next) src->next->pre = src->pre;
    // src->pre = NULL;
    // src->next = NULL;
    return src;
}

inline static list_node_t *__list_node_pop_back(list_node_t *src)
{
    return __list_node_pop_self(__list_node_last(src));
}

inline static list_node_t *__list_node_pop_front(list_node_t *src)
{
    return __list_node_pop_self(__list_node_first(src));
}

inline static list_node_t *__list_node_pop_after(list_node_t *src)
{
    return __list_node_pop_self(src->next);
}

inline static list_node_t *__list_node_pop_before(list_node_t *src)
{
    return __list_node_pop_self(src->pre);
}

inline static void __list_node_swap_data(list_node_t *node1, list_node_t *node2)
{
    void *temp = node1->data;
    node1->data = node2->data;
    node2->data = temp;
}

static void __list_node_reverse(list_node_t *head)
{
    list_node_t *node, *temp;
    for (node = head; node; node = node->pre) {
        temp = node->pre;
        node->pre = node->next;
        node->next = temp;
    }
}

static void __list_node_bubble_sort(list_node_t *head, int(*compare)(void *, void *))
{
    list_node_t *i, *j, *last = NULL;
    for (i = head; i && i->next; i = i->next) {
        for (j = head; j && j->next && j != last; j = j->next) {
            if (compare(j->next->data, j->data) != 0) {
                __list_node_swap_data(j, j->next);
            }
        }
        if (!last) last = j;
        last = last->pre;
    }
}

static void __list_node_select_sort(list_node_t *head, int(*compare)(void *, void *))
{
    list_node_t *i, *j;
    for (i = head; i && i->next; i = i->next) {
        list_node_t *target = i;
        for (j = i->next; j; j = j->next) {
            if (compare(j->data, target->data) != 0)
                target = j;
        }
        if (target != i) {
            __list_node_swap_data(i, target);
        }
    }
}

static void __list_node_insert_sort(list_node_t *head, int(*compare)(void *, void *))
{
    list_node_t *i, *j;
    if (!head) return;
    for (i = head->next; i; i = i->next) {
        void *data = i->data;
        for (j = i->pre; j && compare(data, j->data) != 0; j = j->pre) {
            j->next->data = j->data;
        }
        if (!j) head->data = data;
        else j->next->data = data;
    }
}

#if defined(ENABLE_LIST_NODE_TEST)
static int __compare_int_more_than(void *i1, void *i2)
{
    return *(int *)i1 > *(int *)i2;
}
static int __compare_int_less_than(void *i1, void *i2)
{
    return *(int *)i1 < *(int *)i2;
}
static void __list_node_int_print(list_node_t *head)
{
    list_node_t *node;
    for (node = head; node; node = node->next) {
        printf("%i ", *(int*)node->data);
    }
    printf("\n");
}
void list_node_int_test()
{
    list_node_t *head;
    int int_array[] = { 10, 2, 5, 8, 3, 7, 4, 6, 9, 1 };
    head = __list_node_alloc(&int_array[0]);
    __list_node_push_back(head, __list_node_alloc(&int_array[1]));
    __list_node_push_back(head, __list_node_alloc(&int_array[2]));
    __list_node_push_back(head, __list_node_alloc(&int_array[3]));
    __list_node_push_back(head, __list_node_alloc(&int_array[4]));
    __list_node_push_back(head, __list_node_alloc(&int_array[5]));
    __list_node_push_front(head, __list_node_alloc(&int_array[6]));
    __list_node_push_front(head, __list_node_alloc(&int_array[7]));
    __list_node_push_front(head, __list_node_alloc(&int_array[8]));
    __list_node_push_front(head, __list_node_alloc(&int_array[9]));
    head = __list_node_first(head);
    // print
    __list_node_int_print(head);
    // sort
    __list_node_insert_sort(head, __compare_int_less_than);
    // print
    __list_node_int_print(head);
    // reverse
    __list_node_reverse(head);
    head = __list_node_first(head);
    // print
    __list_node_int_print(head);
    // pop
    list_node_t *pop1 = __list_node_pop_before(head->next->next);
    __list_node_free(pop1);
    // print
    __list_node_int_print(head);
}
#endif

typedef struct list {
    list_node_t *head;
    list_node_t *tail;
    list_node_t *current;
    int size;
} list_t;

list_t *list_alloc()
{
    list_t *list = NULL;
    list = (list_t *)malloc(sizeof(list_t));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void list_free(list_t *list, void(*freecb)(void *))
{
    if (!list) return;
    __list_node_clear(list->head, freecb);
    free(list);
}

void list_clear(list_t *list, void(*freecb)(void *))
{
    if (!list) return;
    __list_node_clear(list->head, freecb);
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

int list_size(list_t *list)
{
    return list->size;
}

void list_reverse(list_t *list)
{
    list_node_t *temp;
    __list_node_reverse(list->head);
    temp = list->head;
    list->head = list->tail;
    list->tail = temp;
}

void list_sort(list_t *list, int(*compare)(void *, void *))
{
#if defined(USE_BUBBLE_SORT)
    __list_node_bubble_sort(list->head, compare);
#elif defined(USE_SELECT_SORT)
    __list_node_select_sort(array, compare);
#elif defined(USE_INSERT_SORT)
    __list_node_insert_sort(array, compare);
#else
    __list_node_bubble_sort(list->head, compare);
#endif
}

void list_push_back(list_t *list, void *data)
{
    list_node_t *node;
    node = __list_node_alloc(data);
    if (!node) return;
    if (list->tail) __list_node_push_after(list->tail, node);
    if (!list->head) list->head = node;
    list->tail = node;
    list->size++;
}

void list_push_front(list_t *list, void *data)
{
    list_node_t *node;
    node = __list_node_alloc(data);
    if (!node) return;
    if (list->head) __list_node_push_before(list->head, node);
    if (!list->tail) list->tail = node;
    list->head = node;
    list->size++;
}

void list_push_at(list_t *list, void *data, int n)
{
    list_node_t *node, *pos;
    pos = __list_node_at(list->head, n);
    if (!pos) return;
    node = __list_node_alloc(data);
    if (!node) return;
    __list_node_push_after(pos, node);
    if (pos == list->tail) list->tail = node;
    list->size++;
}

void list_push(list_t *list, void *data, int(*condition)(void *, void *), void *arg)
{
    list_node_t *node, *new_node;
    for (node = list->head; node; node = node->next) {
        if (condition(node->data, arg) != 0) {
            new_node = __list_node_alloc(data);
            if (new_node) {
                __list_node_push_after(node, new_node);
            }
            break;
        }
    }
}

void *list_pop_back(list_t *list)
{
    list_node_t *popped;
    if (!list->tail) return NULL;
    popped = __list_node_pop_self(list->tail);
    if (--list->size == 0) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->tail = popped->pre;
    }
    if (list->current == popped) {
        list->current = popped->pre;
    }
    return __list_node_free(popped);
}

void *list_pop_front(list_t *list)
{
    list_node_t *popped;
    if (!list->head) return NULL;
    popped = __list_node_pop_self(list->head);
    if (--list->size == 0) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->head = popped->next;
    }
    if (list->current == popped) {
        list->current = popped->pre;
    }
    return __list_node_free(popped);
}

void *list_pop_at(list_t *list, int n)
{
    list_node_t *node, *popped;
    node = __list_node_at(list->head, n);
    if (!node) return NULL;
    popped = __list_node_pop_self(node);
    if (--list->size == 0) {
        list->head = NULL;
        list->tail = NULL;
    } else if (node == list->head) {
        list->head = popped->next;
    } else if (node == list->tail) {
        list->tail = popped->pre;
    }
    if (list->current == popped) {
        list->current = popped->pre;
    }
    return __list_node_free(popped);
}

void *list_pop(list_t *list, int(*condition)(void *, void *), void *arg)
{
    list_node_t *node = NULL;
    for (node = list->head; node; node = node->next) {
        if (condition(node->data, arg) != 0) {
            if (list->current == node) {
                list->current = node->pre;
            }
            return __list_node_free(node);
        }
    }
    return NULL;
}

void *list_front(list_t *list)
{
    if (!list->head) return NULL;
    return list->head->data;
}

void *list_back(list_t *list)
{
    if (!list->tail) return NULL;
    return list->tail->data;
}

void *list_begin(list_t *list)
{
    list->current = list->head;
    if (!list->current) return NULL;
    return list->current->data;
}

void *list_end(list_t *list)
{
    return NULL;
}

void *list_at(list_t *list, int n)
{
    list_node_t *node;
    node = __list_node_at(list->head, n);
    if (!node) return NULL;
    list->current = node;
    return list->current->data;
}

void *list_next(list_t *list)
{
    if (!list->current) return NULL;
    list->current = list->current->next;
    if (!list->current) return NULL;
    return list->current->data;
}

void list_foreach(list_t *list, void(*cb)(void *, void *), void *arg)
{
    list_node_t *node = NULL;
    for (node = list->head; node; node = node->next) {
        cb(node->data, arg);
    }
}

#if defined(ENABLE_LIST_TEST)
static int __compare_int_more_than(void *i1, void *i2)
{
    return *(int *)i1 > *(int *)i2;
}
static int __compare_int_less_than(void *i1, void *i2)
{
    return *(int *)i1 < *(int *)i2;
}
static void list_print(list_t *list)
{
    int *data;
    for (data = (int *)list_begin(list); data; data = (int *)list_next(list)) {
        printf("%d ", *data);
    }
    printf("\n");
}
static void list_print_cb(void *data, void *arg)
{
    printf("%d ", *(int *)data);
}
void list_int_test()
{
    list_t *list = list_alloc();
    int int_array[] = { 2,5,6,4,3,1,8,9,7,0 };
    list_push_back(list, &int_array[0]);
    list_push_back(list, &int_array[1]);
    list_push_back(list, &int_array[2]);
    list_push_back(list, &int_array[3]);
    list_push_back(list, &int_array[4]);
    list_push_front(list, &int_array[5]);
    list_push_front(list, &int_array[6]);
    list_push_front(list, &int_array[7]);
    list_push_front(list, &int_array[8]);
    list_push_front(list, &int_array[9]);
    assert(list_size(list) == 10);
    list_sort(list, __compare_int_less_than);
    assert(*(int*)list_at(list, 6) == 6);
    assert(*(int*)list_begin(list) == 0);
    assert(*(int*)list_next(list) == 1);
    assert(*(int*)list_next(list) == 2);
    assert(*(int*)list_next(list) == 3);
    assert(*(int*)list_next(list) == 4);
    list_pop_back(list);
    list_pop_back(list);
    list_pop_back(list);
    list_pop_back(list);
    assert(list_size(list) == 6);
    list_pop_front(list);
    assert(list_size(list) == 5);
    assert(*(int*)list_begin(list) == 1);
    assert(*(int*)list_next(list) == 2);
    assert(*(int*)list_next(list) == 3);
    assert(*(int*)list_next(list) == 4);
    assert(*(int*)list_next(list) == 5);
    list_reverse(list);
    assert(*(int*)list_begin(list) == 5);
    assert(*(int*)list_next(list) == 4);
    assert(*(int*)list_next(list) == 3);
    assert(*(int*)list_next(list) == 2);
    assert(*(int*)list_next(list) == 1);
    list_print(list);
    list_foreach(list, list_print_cb, NULL);
    printf("\n");
    list_free(list, NULL);
}
#endif