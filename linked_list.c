#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "linked_list.h"

#ifndef NULL
#define NULL (void *)0
#endif

inline static struct linked_list_node *__list_node_alloc(void *data)
{
    struct linked_list_node *node;
    node = (struct linked_list_node *)malloc(sizeof(struct linked_list_node));
    if (!node) return NULL;
    node->data = data;
    node->pre = NULL;
    node->next = NULL;
    return node;
}

inline static void *__list_node_free(struct linked_list_node *src)
{
    void *data = src->data;
    free(src);
    return data;
}

inline static void __list_node_clear(struct linked_list_node *head, void(*freecb)(void *))
{
    struct linked_list_node *node, *temp = NULL;
    for (node = head; node && node->next; node = temp) {
        if (freecb) freecb(node->data);
        temp = node->next;
        free(node);
    }
}

inline static struct linked_list_node *__list_node_first(struct linked_list_node *src)
{
    struct linked_list_node *node = NULL;
    for (node = src; node && node->pre; node = node->pre);
    return node;
}

inline static struct linked_list_node *__list_node_last(struct linked_list_node *src)
{
    struct linked_list_node *node = NULL;
    for (node = src; node && node->next; node = node->next);
    return node;
}

inline static struct linked_list_node *__list_node_at(struct linked_list_node *src, int n)
{
    int i = 0;
    struct linked_list_node *node = NULL;
    for (node = src; node && node->next && i < n; node = node->next, i++);
    return node;
}

inline static void __list_node_push_after(struct linked_list_node *src, struct linked_list_node *node)
{
    if (!src || !node) return;
    if (src->next) src->next->pre = node;
    node->next = src->next;
    node->pre = src;
    src->next = node;
}

inline static void __list_node_push_before(struct linked_list_node *src, struct linked_list_node *node)
{
    if (!src || !node) return;
    if (src->pre) src->pre->next = node;
    node->pre = src->pre;
    node->next = src;
    src->pre = node;
}

inline static void __list_node_push_back(struct linked_list_node *src, struct linked_list_node *node)
{
    __list_node_push_after(__list_node_last(src), node);
}

inline static void __list_node_push_front(struct linked_list_node *src, struct linked_list_node *node)
{
    __list_node_push_before(__list_node_first(src), node);
}

inline static struct linked_list_node *__list_node_pop_self(struct linked_list_node *src)
{
    if (!src) return NULL;
    if (src->pre) src->pre->next = src->next;
    if (src->next) src->next->pre = src->pre;
    // src->pre = NULL;
    // src->next = NULL;
    return src;
}

inline static struct linked_list_node *__list_node_pop_back(struct linked_list_node *src)
{
    return __list_node_pop_self(__list_node_last(src));
}

inline static struct linked_list_node *__list_node_pop_front(struct linked_list_node *src)
{
    return __list_node_pop_self(__list_node_first(src));
}

inline static struct linked_list_node *__list_node_pop_after(struct linked_list_node *src)
{
    return __list_node_pop_self(src->next);
}

inline static struct linked_list_node *__list_node_pop_before(struct linked_list_node *src)
{
    return __list_node_pop_self(src->pre);
}

inline static void __list_node_swap_data(struct linked_list_node *node1, struct linked_list_node *node2)
{
    void *temp = node1->data;
    node1->data = node2->data;
    node2->data = temp;
}

static void __list_node_reverse(struct linked_list_node *head)
{
    struct linked_list_node *node, *temp;
    for (node = head; node; node = node->pre) {
        temp = node->pre;
        node->pre = node->next;
        node->next = temp;
    }
}

static void __list_node_bubble_sort(struct linked_list_node *head, int(*compare)(void *, void *))
{
    struct linked_list_node *i, *j, *last = NULL;
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

static void __list_node_select_sort(struct linked_list_node *head, int(*compare)(void *, void *))
{
    struct linked_list_node *i, *j;
    for (i = head; i && i->next; i = i->next) {
        struct linked_list_node *target = i;
        for (j = i->next; j; j = j->next) {
            if (compare(j->data, target->data) != 0)
                target = j;
        }
        if (target != i) {
            __list_node_swap_data(i, target);
        }
    }
}

static void __list_node_insert_sort(struct linked_list_node *head, int(*compare)(void *, void *))
{
    struct linked_list_node *i, *j;
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

#if defined(ENABLE_LINKED_LIST_TEST)
static int __compare_int_more_than(void *i1, void *i2)
{
    return *(int *)i1 > *(int *)i2;
}
static int __compare_int_less_than(void *i1, void *i2)
{
    return *(int *)i1 < *(int *)i2;
}
static void __list_node_int_print(struct linked_list_node *head)
{
    struct linked_list_node *node;
    for (node = head; node; node = node->next) {
        printf("%i ", *(int*)node->data);
    }
    printf("\n");
}
void linked_list_node_int_test()
{
    struct linked_list_node *head;
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
    struct linked_list_node *pop1 = __list_node_pop_before(head->next->next);
    __list_node_free(pop1);
    // print
    __list_node_int_print(head);
}
#endif

struct linked_list *linked_list_alloc()
{
    struct linked_list *list = NULL;
    list = (struct linked_list *)malloc(sizeof(struct linked_list));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void linked_list_free(struct linked_list *list, void(*datacb)(void *))
{
    if (!list) return;
    __list_node_clear(list->head, freecb);
    free(list);
}

void linked_list_clear(struct linked_list *list, void(*freecb)(void *))
{
    if (!list) return;
    __list_node_clear(list->head, freecb);
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

int linked_list_size(struct linked_list *list)
{
    return list->size;
}

void linked_list_reverse(struct linked_list *list)
{
    struct linked_list_node *temp;
    __list_node_reverse(list->head);
    temp = list->head;
    list->head = list->tail;
    list->tail = temp;
}

void linked_list_sort(struct linked_list *list, int(*compare)(void *, void *))
{
    __list_node_bubble_sort(list->head, compare);
}

void linked_list_push_back(struct linked_list *list, void *data)
{
    struct linked_list_node *node;
    node = __list_node_alloc(data);
    if (!node) return;
    if (list->tail) __list_node_push_after(list->tail, node);
    if (!list->head) list->head = node;
    list->tail = node;
    list->size++;
}

void linked_list_push_front(struct linked_list *list, void *data)
{
    struct linked_list_node *node;
    node = __list_node_alloc(data);
    if (!node) return;
    if (list->head) __list_node_push_before(list->head, node);
    if (!list->tail) list->tail = node;
    list->head = node;
    list->size++;
}

void linked_list_push_at(struct linked_list *list, void *data, int n)
{
    struct linked_list_node *node, *pos;
    pos = __list_node_at(list->head, n);
    if (!pos) return;
    node = __list_node_alloc(data);
    if (!node) return;
    __list_node_push_after(pos, node);
    if (pos == list->tail) list->tail = node;
    list->size++;
}

void linked_list_push(struct linked_list *list, void *data, int(*condition)(void *, void *), void *arg)
{
    struct linked_list_node *node, *new_node;
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

void *linked_list_pop_back(struct linked_list *list)
{
    struct linked_list_node *popped;
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

void *linked_list_pop_front(struct linked_list *list)
{
    struct linked_list_node *popped;
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

void *linked_list_pop_at(struct linked_list *list, int n)
{
    struct linked_list_node *node, *popped;
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

void *linked_list_pop(struct linked_list *list, int(*condition)(void *, void *), void *arg)
{
    struct linked_list_node *node = NULL;
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

void *linked_list_begin(struct linked_list *list)
{
    list->current = list->head;
    if (!list->current) return NULL;
    return list->current->data;
}

void *linked_list_end(struct linked_list *list)
{
    return NULL;
}

void *linked_list_at(struct linked_list *list, int n)
{
    struct linked_list_node *node;
    node = __list_node_at(list->head, n);
    if (!node) return NULL;
    list->current = node;
    return list->current->data;
}

void *linked_list_next(struct linked_list *list)
{
    if (!list->current) return NULL;
    list->current = list->current->next;
    if (!list->current) return NULL;
    return list->current->data;
}

void linked_list_foreach(struct linked_list *list, void(*cb)(void *, void *), void *arg)
{
    struct linked_list_node *node = NULL;
    for (node = list->head; node; node = node->next) {
        cb(node->data, arg);
    }
}

#if defined(ENABLE_LINKED_LIST_TEST)
static void linked_list_print(struct linked_list *list)
{
    int *data;
    for (data = (int *)linked_list_begin(list); data; data = (int *)linked_list_next(list)) {
        printf("%d ", *data);
    }
    printf("\n");
}
static void linked_list_print_cb(void *data, void *arg)
{
    printf("%d ", *(int *)data);
}
void linked_list_int_test()
{
    struct linked_list *list = linked_list_alloc();
    int int_array[] = { 2,5,6,4,3,1,8,9,7,0 };
    linked_list_push_back(list, &int_array[0]);
    linked_list_push_back(list, &int_array[1]);
    linked_list_push_back(list, &int_array[2]);
    linked_list_push_back(list, &int_array[3]);
    linked_list_push_back(list, &int_array[4]);
    linked_list_push_front(list, &int_array[5]);
    linked_list_push_front(list, &int_array[6]);
    linked_list_push_front(list, &int_array[7]);
    linked_list_push_front(list, &int_array[8]);
    linked_list_push_front(list, &int_array[9]);
    assert(linked_list_size(list) == 10);
    linked_list_sort(list, __compare_int_less_than);
    assert(*(int*)linked_list_at(list, 6) == 6);
    assert(*(int*)linked_list_begin(list) == 0);
    assert(*(int*)linked_list_next(list) == 1);
    assert(*(int*)linked_list_next(list) == 2);
    assert(*(int*)linked_list_next(list) == 3);
    assert(*(int*)linked_list_next(list) == 4);
    linked_list_pop_back(list);
    linked_list_pop_back(list);
    linked_list_pop_back(list);
    linked_list_pop_back(list);
    assert(linked_list_size(list) == 6);
    linked_list_pop_front(list);
    assert(linked_list_size(list) == 5);
    assert(*(int*)linked_list_begin(list) == 1);
    assert(*(int*)linked_list_next(list) == 2);
    assert(*(int*)linked_list_next(list) == 3);
    assert(*(int*)linked_list_next(list) == 4);
    assert(*(int*)linked_list_next(list) == 5);
    linked_list_reverse(list);
    assert(*(int*)linked_list_begin(list) == 5);
    assert(*(int*)linked_list_next(list) == 4);
    assert(*(int*)linked_list_next(list) == 3);
    assert(*(int*)linked_list_next(list) == 2);
    assert(*(int*)linked_list_next(list) == 1);
    linked_list_print(list);
    linked_list_foreach(list, linked_list_print_cb, NULL);
    printf("\n");
    linked_list_free(list, NULL);
}
#endif