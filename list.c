#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "list.h"

static struct list_node *list_node_alloc__(void *data, int size)
{
    struct list_node *node;
    node = (struct list_node *)malloc(sizeof(struct list_node));
    if (!node) {
        return NULL;
    }
    node->data = (char *)malloc(size);
    if (!node->data) {
        free(node);
        return NULL;
    }
    memcpy(node->data, data, size);
    node->next = NULL;
    node->pre = NULL;
    return node;
}

static void list_node_free__(struct list_node *node) 
{
    if (node) {
        free(node->data);
        free(node);
    }
}

struct list *list_alloc(int sizeof_type)
{
    struct list *list;
    list = (struct list *)malloc(sizeof(struct list));
    if (!list) {
        fprintf(stderr, "alloc error \n");
        return NULL;
    }
    list->size = 0;
    list->sizeof_type = sizeof_type;
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void list_free(struct list *list)
{
    if (list) {
        list_clear(list);
        free(list);
    }
}

void list_clear(struct list *list)
{
    assert(list);
    struct list_node *temp, *node;
    for (node = list->head; node; node = temp) {
        temp = node->next;
        list_node_free__(node);
    }
    list->head = NULL;
    list->tail = NULL;
}

int list_push_at(struct list *list, void *data, int index)
{
    int i = 0;
    struct list_node *temp, *node;

    node = list_node_alloc__(data, list->sizeof_type);
    if (!node) {
        return 0;
    }

    // first node
    if (!list->head || !list->tail) {
        list->tail = node;
        list->head = node;
        list->size = 1;
        return 1;
    }
    // tail
    for (temp = list->head; temp && i < index; temp = temp->next, i++) {
        if (!temp->next) {
            node->pre = temp;
            temp->next = node;
            list->tail = node;
            list->size++;
            return 1;
        }
    }
    // head
    if (temp == list->head) {
        node->next = temp;
        node->next->pre = node;
        list->head = node;
        list->size++;
        return 1;
    }
    // others
    node->pre = temp->pre;
    node->pre->next = node;
    node->next = temp;
    node->next->pre = node;
    list->size++;
    return 1;
}

int list_push_back(struct list *list, void *data)
{
    struct list_node *node;
    
    node = list_node_alloc__(data, list->sizeof_type);
    if (!node) {
        return 0;
    }
    // first node
    if (!list->head || !list->tail) {
        list->tail = node;
        list->head = node;
        list->size = 1;
    } else {
        node->pre = list->tail;
        node->pre->next = node;
        list->tail = node;
        list->size++;
    }
    return 1;
}

int list_push_front(struct list *list, void *data)
{
    struct list_node *node;

    node = list_node_alloc__(data, list->sizeof_type);
    if (!node) {
        return 0;
    }
    // first node
    if (!list->head || !list->tail) {
        list->tail = node;
        list->head = node;
        list->size = 1;
    } else {
        node->next = list->head;
        node->next->pre = node;
        list->head = node;
        list->size++;
    }
    return 1;
}

int list_pop_at(struct list *list, int index)
{
    int i = 0;
    struct list_node *node;

    for (node = list->head; node && node->next; node = node->next, i++) {
        if (i == index) {
            break;
        }
    }
    if (!node || i != index) {
        return 0;
    }
    if (!node->pre) {
        list->head = node->next;
    }
    if (!node->next) {
        list->tail = node->pre;
    }
    if (node->pre) {
        node->pre->next = node->next;
    }
    if (node->next) {
        node->next->pre = node->pre;
    }
    list_node_free__(node);
    list->size--;
    return 1;
}

int list_pop_back(struct list *list)
{
    struct list_node *node = list->tail;
    if (!node) {
        // zero
        return 0;
    } 
    if (!node->pre) {
        // only one
        list->head = NULL; //
        list->tail = NULL;
    } else {
        list->tail = node->pre;
        list->tail->next = NULL;
    }
    list_node_free__(node);
    list->size--;
    return 1;
}

int list_pop_front(struct list *list)
{
    struct list_node *node = list->head;
    if (!node) {
        // zero
        return 0;
    } 
    if (!node->next) {
        // only one
        list->head = NULL; //
        list->tail = NULL;
    } else {
        list->head = node->next;
        list->head->pre = NULL;
    }
    list_node_free__(node);
    list->size--;
    return 1;
}

void *list_begin(struct list *list)
{
    return list->head;
}

void *list_end(struct list *list)
{
    return list->tail;
}

void *list_at(struct list *list, int index)
{
    int i = 0;
    struct list_node *node = list->head;
    for (; node && i < index; node = node->next, i++);
    return node;
}

void list_foreach(struct list *list, int(*cb)(void *, void *), void *arg)
{
    struct list_node *node;
    for (node = list->head; node; node = node->next) {
        if (cb) {
            cb(node->data, arg);
        }
    }
}

int list_empty(struct list *list)
{
    return list->size == 0 ? 1 : 0;
}

int list_size(struct list *list)
{
    return list->size;
}
