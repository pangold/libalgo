#ifndef __LIST_NODE_H__
#define __LIST_NODE_H__

typedef struct single_list_node {
    void *data;
    struct single_list_node *next;
} single_list_node_t, scircle_list_node_t;

typedef struct double_list_node {
    void *data;
    struct double_list_node *pre;
    struct double_list_node *next;
} double_list_node_t, dcircle_list_node_t;

static single_list_node_t *single_list_node_alloc(void *data);
static double_list_node_t *double_list_node_alloc(void *data);
static scircle_list_node_t *scircle_list_node_alloc(void *data);
static dcircle_list_node_t *dcircle_list_node_alloc(void *data);

static void single_list_node_clear(single_list_node_t *head, void(*dfree)(void *));
static void double_list_node_clear(double_list_node_t *head, void(*dfree)(void *));
static void scircle_list_node_clear(scircle_list_node_t *head, void(*dfree)(void *));
static void dcircle_list_node_clear(dcircle_list_node_t *head, void(*dfree)(void *));

static single_list_node_t *single_list_node_find(single_list_node_t *head, void *data, int(*compare)(void *, void *));
static double_list_node_t *double_list_node_find(double_list_node_t *head, void *data, int(*compare)(void *, void *));
static scircle_list_node_t *scircle_list_node_find(scircle_list_node_t *head, void *data, int(*compare)(void *, void *));
static dcircle_list_node_t *dcircle_list_node_find(dcircle_list_node_t *head, void *data, int(*compare)(void *, void *));

static single_list_node_t *single_list_node_at(single_list_node_t *head, int n);
static double_list_node_t *double_list_node_at(double_list_node_t *head, int n);
static scircle_list_node_t *scircle_list_node_at(scircle_list_node_t *head, int n);
static dcircle_list_node_t *dcircle_list_node_at(dcircle_list_node_t *head, int n);

static single_list_node_t *single_list_node_last(single_list_node_t *head);
static double_list_node_t *double_list_node_last(double_list_node_t *head);
static scircle_list_node_t *scircle_list_node_last(scircle_list_node_t *head);
static dcircle_list_node_t *dcircle_list_node_last(dcircle_list_node_t *head);

static int single_list_node_size(single_list_node_t *head);
static int double_list_node_size(double_list_node_t *head);
static int scircle_list_node_size(scircle_list_node_t *head);
static int dcircle_list_node_size(dcircle_list_node_t *head);

static void single_list_node_push_after(single_list_node_t *node, single_list_node_t *next);
static void double_list_node_push_after(double_list_node_t *node, double_list_node_t *next);
static void scircle_list_node_push_after(scircle_list_node_t *node, scircle_list_node_t *next);
static void dcircle_list_node_push_after(dcircle_list_node_t *node, dcircle_list_node_t *next);

static void single_list_node_push_before(single_list_node_t *node, single_list_node_t *pre);
static void double_list_node_push_before(double_list_node_t *node, double_list_node_t *pre);
// not efficient
static void scircle_list_node_push_before(scircle_list_node_t *node, scircle_list_node_t *pre);
static void dcircle_list_node_push_before(dcircle_list_node_t *node, dcircle_list_node_t *pre);

static void single_list_node_push_back(single_list_node_t *head, single_list_node_t *node);
static void double_list_node_push_back(double_list_node_t *head, double_list_node_t *node);
static void scircle_list_node_push_back(scircle_list_node_t *head, scircle_list_node_t *node);
static void dcircle_list_node_push_back(dcircle_list_node_t *head, dcircle_list_node_t *node);

static void single_list_node_push_front(single_list_node_t *head, single_list_node_t *node);
static void double_list_node_push_front(double_list_node_t *head, double_list_node_t *node);
static void scircle_list_node_push_front(scircle_list_node_t *head, scircle_list_node_t *node);
static void dcircle_list_node_push_front(dcircle_list_node_t *head, dcircle_list_node_t *node);

static single_list_node_t *single_list_node_reverse(single_list_node_t *head);
static double_list_node_t *double_list_node_reverse(double_list_node_t *head);
static scircle_list_node_t *scircle_list_node_reverse(scircle_list_node_t *head);
static dcircle_list_node_t *dcircle_list_node_reverse(dcircle_list_node_t *head);

static single_list_node_t *single_list_node_sort(single_list_node_t *head);

#endif