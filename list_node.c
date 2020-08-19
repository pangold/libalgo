#include <stdio.h>
#include <assert.h>

typedef struct single_list_node {
    void *data;
    struct single_list_node *next;
} single_list_node_t, scircle_list_node_t;

typedef struct double_list_node {
    void *data;
    struct double_list_node *pre;
    struct double_list_node *next;
} double_list_node_t, dcircle_list_node_t;


/**
 * alloc API
 */
inline static single_list_node_t *single_list_node_alloc(void *data)
{
    single_list_node_t *node;
    node = (single_list_node_t *)malloc(sizeof(single_list_node_t));
    if (!node) return NULL;
    node->data = data;
    node->next = NULL;
    return node;
}

inline static double_list_node_t *double_list_node_alloc(void *data)
{
    double_list_node_t *node;
    node = (double_list_node_t *)malloc(sizeof(double_list_node_t));
    if (!node) return NULL;
    node->data = data;
    node->next = NULL;
    node->pre = NULL;
    return node;
}

inline static scircle_list_node_t *scircle_list_node_alloc(void *data)
{
    scircle_list_node_t *node;
    node = (scircle_list_node_t *)malloc(sizeof(scircle_list_node_t));
    if (!node) return NULL;
    node->data = data;
    node->next = node;
    return node;
}

inline static dcircle_list_node_t *dcircle_list_node_alloc(void *data)
{
    dcircle_list_node_t *node;
    node = (dcircle_list_node_t *)malloc(sizeof(dcircle_list_node_t));
    if (!node) return NULL;
    node->data = data;
    node->next = node;
    node->pre = node;
    return node;
}

/**
 * clear API
 */
#define node_free(node, dfree) \
    if ((node)->data) \
        dfree((node)->data); \
    free(node)

inline static void single_list_node_clear(single_list_node_t *head, void(*dfree)(void *))
{
    if (!head) return;
    single_list_node_t *next, *node;
    for (node = head; node; node = next) {
        next = node->next;
        node_free(node, dfree);
    }
}

inline static void double_list_node_clear(double_list_node_t *head, void(*dfree)(void *))
{
    if (!head) return;
    double_list_node_t *next, *node;
    for (node = head; node; node = next) {
        next = node->next;
        node_free(node, dfree);
    }
}

inline static void scircle_list_node_clear(scircle_list_node_t *head, void(*dfree)(void *))
{
    if (!head) return;
    scircle_list_node_t *next, *node = head;
    do {
        next = node->next;
        node_free(node, dfree);
        node = next;
    } while (node != head);

    // for (node = head; node->next != head; node = next) {
    //     next = node->next;
    //     node_free(node, dfree);
    // }
    // node_free(node, dfree);
}

inline static void dcircle_list_node_clear(dcircle_list_node_t *head, void(*dfree)(void *))
{
    if (!head) return;
    dcircle_list_node_t *next, *node = head;
    do {
        next = node->next;
        node_free(node, dfree);
        node = next;
    } while (node != head);
}

/**
 * find API
 */
inline static single_list_node_t *single_list_node_find(single_list_node_t *head, void *data, int(*compare)(void *, void *))
{
    single_list_node_t *node;
    for (node = head; node; node = node->next) {
        if (compare(node->data, data) == 0) {
            return node;
        }
    }
    return NULL;
}

inline static double_list_node_t *double_list_node_find(double_list_node_t *head, void *data, int(*compare)(void *, void *))
{
    single_list_node_t *node;
    for (node = head; node; node = node->next) {
        if (compare(node->data, data) == 0) {
            return node;
        }
    }
    return NULL;
}

inline static scircle_list_node_t *scircle_list_node_find(scircle_list_node_t *head, void *data, int(*compare)(void *, void *))
{
    single_list_node_t *node;
    for (node = head; node; node = node->next) {
        if (compare(node->data, data) == 0) {
            return node;
        }
    }
    return NULL;
}

inline static dcircle_list_node_t *dcircle_list_node_find(dcircle_list_node_t *head, void *data, int(*compare)(void *, void *))
{
    single_list_node_t *node;
    for (node = head; node; node = node->next) {
        if (compare(node->data, data) == 0) {
            return node;
        }
    }
    return NULL;
}

/**
 * at API
 * @return expected node or NULL if exceed the tail
 */
inline static single_list_node_t *single_list_node_at(single_list_node_t *head, int n)
{
    if (!head) return NULL;
    single_list_node_t *node;
    for (node = head; node && n > 0; node = node->next, n--);
    return node;
}

inline static double_list_node_t *double_list_node_at(double_list_node_t *head, int n)
{
    if (!head) return NULL;
    double_list_node_t *node;
    for (node = head; node && n > 0; node = node->next, n--);
    return node;
}

inline static scircle_list_node_t *scircle_list_node_at(scircle_list_node_t *head, int n)
{
    if (!head) return NULL;
    if (n == 0) return head;
    scircle_list_node_t *node = head;
    do {
        node = node->next;
        n--;
    } while (node != head && n > 0);
    return node != head ? node : NULL;
}

inline static dcircle_list_node_t *dcircle_list_node_at(dcircle_list_node_t *head, int n)
{
    if (!head) return NULL;
    if (n == 0) return head;
    dcircle_list_node_t *node = head;
    do {
        node = node->next;
        n--;
    } while (node != head && n > 0);
    return node != head ? node : NULL;
}

/**
 * last API
 */
inline static single_list_node_t *single_list_node_last(single_list_node_t *head)
{
    if (!head) return NULL;
    single_list_node_t *node;
    for (node = head; node->next; node = node->next);
    return node;
}

inline static double_list_node_t *double_list_node_last(double_list_node_t *head)
{
    if (!head) return NULL;
    double_list_node_t *node;
    for (node = head; node->next; node = node->next);
    return node;
}

inline static scircle_list_node_t *scircle_list_node_last(scircle_list_node_t *head)
{
    if (!head) return NULL;
    scircle_list_node_t *node = head;
    for (node = head->next; node->next != head; node = node->next);
    return node;
}

inline static dcircle_list_node_t *dcircle_list_node_last(dcircle_list_node_t *head)
{
    if (!head) return NULL;
    return head->pre;
}

/**
 * push after API
 */
inline static void single_list_node_push_after(single_list_node_t *node, single_list_node_t *next)
{
    single_list_node_t *temp;
    temp = node->next;
    node->next = next;
    next->next = temp;
}

inline static void double_list_node_push_after(double_list_node_t *node, double_list_node_t *next)
{
    double_list_node_t *temp;
    temp = node->next;
    node->next = next;
    next->next = temp;
    node->next->pre = node;
    if (next->next) next->next->pre = next;
}

inline static void scircle_list_node_push_after(scircle_list_node_t *node, scircle_list_node_t *next)
{
    scircle_list_node_t *temp;
    temp = node->next;
    node->next = next;
    next->next = temp;
}

inline static void dcircle_list_node_push_after(dcircle_list_node_t *node, dcircle_list_node_t *next)
{
    dcircle_list_node_t *temp;
    temp = node->next;
    node->next = next;
    next->next = temp;
    node->next->pre = node;
    next->next->pre = next;
}

/**
 * push before API
 */
inline static void single_list_node_push_before(single_list_node_t *node, single_list_node_t *pre)
{
    pre->next = node;
}

inline static void double_list_node_push_before(double_list_node_t *node, double_list_node_t *pre)
{
    double_list_node_t *temp;
    temp = node->pre;
    node->pre = pre;
    pre->pre = temp;
    node->pre->next = node;
    if (pre->pre) pre->pre->next = pre;
}

// not efficient
inline static void scircle_list_node_push_before(scircle_list_node_t *node, scircle_list_node_t *pre)
{
    scircle_list_node_t *temp;
    temp = scircle_list_node_last(node);
    scircle_list_node_push_after(temp, pre);
}

inline static void dcircle_list_node_push_before(dcircle_list_node_t *node, dcircle_list_node_t *pre)
{
    dcircle_list_node_t *temp;
    temp = node->pre;
    node->pre = pre;
    pre->pre = temp;
    node->pre->next = node;
    pre->pre->next = pre;
}

/**
 * push back API
 */
inline static void single_list_node_push_back(single_list_node_t *head, single_list_node_t *node)
{
    single_list_node_t *temp;
    temp = single_list_node_last(head);
    single_list_node_push_after(temp, node);
}

inline static void double_list_node_push_back(double_list_node_t *head, double_list_node_t *node)
{
    double_list_node_t *temp;
    temp = double_list_node_last(head);
    double_list_node_push_after(temp, node);
}

inline static void scircle_list_node_push_back(scircle_list_node_t *head, scircle_list_node_t *node)
{
    scircle_list_node_push_before(head, node);
}

inline static void dcircle_list_node_push_back(dcircle_list_node_t *head, dcircle_list_node_t *node)
{
    dcircle_list_node_push_before(head, node);
}

/**
 * push front API
 */
inline static void single_list_node_push_front(single_list_node_t *head, single_list_node_t *node)
{
    single_list_node_push_before(head, node);
}

inline static void double_list_node_push_front(double_list_node_t *head, double_list_node_t *node)
{
    double_list_node_push_before(head, node);
}

inline static void scircle_list_node_push_front(scircle_list_node_t *head, scircle_list_node_t *node)
{
    scircle_list_node_push_before(head, node);
}

inline static void dcircle_list_node_push_front(dcircle_list_node_t *head, dcircle_list_node_t *node)
{
    dcircle_list_node_push_before(head, node);
}

/**
 * reverse API
 * @return new head
 */
inline static single_list_node_t *single_list_node_reverse(single_list_node_t *head)
{
    if (!head || !head->next) return head;
    single_list_node_t *pre = NULL;
    single_list_node_t *cur = head;
    single_list_node_t *next = head->next;
    for ( ; next; pre = cur, cur = next, next = next->next) {
        cur->next = pre;
    }
    cur->next = pre;
    return cur;
}

inline static double_list_node_t *double_list_node_reverse(double_list_node_t *head)
{
    if (!head) return head;
    double_list_node_t *tmp = NULL;
    double_list_node_t *pre = NULL;
    double_list_node_t *cur = head;
    for ( ; cur; pre = cur, cur = cur->next) {
        tmp = cur->next;
        cur->next = cur->pre;
        cur->pre = tmp;
    }
    return pre;
}

inline static scircle_list_node_t *scircle_list_node_reverse(scircle_list_node_t *head)
{
    if (!head || head->next == head) return head;
    scircle_list_node_t *pre = NULL;
    scircle_list_node_t *cur = head;
    scircle_list_node_t *next = head->next;
    for ( ; next != head; pre = cur, cur = next, next = next->next) {
        cur->next = pre;
    }
    cur->next = pre;
    return cur;
}

inline static dcircle_list_node_t *dcircle_list_node_reverse(dcircle_list_node_t *head)
{
    if (!head) return head;
    dcircle_list_node_t *tmp = NULL;
    dcircle_list_node_t *cur = head;
    dcircle_list_node_t *next = head->next;
    for ( ; next != head; cur = cur->next, next = next->next) {
        // swap node->pre and node->next
        tmp = cur->next;
        cur->next = cur->pre;
        cur->pre = tmp;
    }
    tmp = cur->next;
    cur->next = cur->pre;
    cur->pre = tmp;
    return cur;
}

