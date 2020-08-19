#ifndef __LIST_NODE_H__
#define __LIST_NODE_H__


typedef struct single_list_node single_list_node_t, scircle_list_node_t;
typedef struct double_list_node double_list_node_t, dcircle_list_node_t;

/**
 * alloc API
 */
inline static single_list_node_t *single_list_node_alloc(void *data);
inline static double_list_node_t *double_list_node_alloc(void *data);
inline static scircle_list_node_t *scircle_list_node_alloc(void *data);
inline static dcircle_list_node_t *dcircle_list_node_alloc(void *data);

/**
 * clear API
 */
inline static void single_list_node_clear(single_list_node_t *head, void(*dfree)(void *));
inline static void double_list_node_clear(double_list_node_t *head, void(*dfree)(void *));
inline static void scircle_list_node_clear(scircle_list_node_t *head, void(*dfree)(void *));
inline static void dcircle_list_node_clear(dcircle_list_node_t *head, void(*dfree)(void *));

/**
 * find API
 */
inline static single_list_node_t *single_list_node_find(single_list_node_t *head, void *data, int(*compare)(void *, void *));
inline static double_list_node_t *double_list_node_find(double_list_node_t *head, void *data, int(*compare)(void *, void *));
inline static scircle_list_node_t *scircle_list_node_find(scircle_list_node_t *head, void *data, int(*compare)(void *, void *));
inline static dcircle_list_node_t *dcircle_list_node_find(dcircle_list_node_t *head, void *data, int(*compare)(void *, void *));

/** 
 * at API
 * @return expected node or NULL if exceed the tail
 */
inline static single_list_node_t *single_list_node_at(single_list_node_t *head, int n);
inline static double_list_node_t *double_list_node_at(double_list_node_t *head, int n);
inline static scircle_list_node_t *scircle_list_node_at(scircle_list_node_t *head, int n);
inline static dcircle_list_node_t *dcircle_list_node_at(dcircle_list_node_t *head, int n);

/**
 * last API
 */
inline static single_list_node_t *single_list_node_last(single_list_node_t *head);
inline static double_list_node_t *double_list_node_last(double_list_node_t *head);
inline static scircle_list_node_t *scircle_list_node_last(scircle_list_node_t *head);
inline static dcircle_list_node_t *dcircle_list_node_last(dcircle_list_node_t *head);

/**
 * push after API
 */
inline static void single_list_node_push_after(single_list_node_t *node, single_list_node_t *next);
inline static void double_list_node_push_after(double_list_node_t *node, double_list_node_t *next);
inline static void scircle_list_node_push_after(scircle_list_node_t *node, scircle_list_node_t *next);
inline static void dcircle_list_node_push_after(dcircle_list_node_t *node, dcircle_list_node_t *next);

/**
 * push before API
 */
inline static void single_list_node_push_before(single_list_node_t *node, single_list_node_t *pre);
inline static void double_list_node_push_before(double_list_node_t *node, double_list_node_t *pre);
// not efficient
inline static void scircle_list_node_push_before(scircle_list_node_t *node, scircle_list_node_t *pre);
inline static void dcircle_list_node_push_before(dcircle_list_node_t *node, dcircle_list_node_t *pre);

/**
 * push back API
 */
inline static void single_list_node_push_back(single_list_node_t *head, single_list_node_t *node);
inline static void double_list_node_push_back(double_list_node_t *head, double_list_node_t *node);
inline static void scircle_list_node_push_back(scircle_list_node_t *head, scircle_list_node_t *node);
inline static void dcircle_list_node_push_back(dcircle_list_node_t *head, dcircle_list_node_t *node);

/**
 * push front API
 */
inline static void single_list_node_push_front(single_list_node_t *head, single_list_node_t *node);
inline static void double_list_node_push_front(double_list_node_t *head, double_list_node_t *node);
inline static void scircle_list_node_push_front(scircle_list_node_t *head, scircle_list_node_t *node);
inline static void dcircle_list_node_push_front(dcircle_list_node_t *head, dcircle_list_node_t *node);

/** 
 * reverse API
 * @return new head
 */
inline static single_list_node_t *single_list_node_reverse(single_list_node_t *head);
inline static double_list_node_t *double_list_node_reverse(double_list_node_t *head);
inline static scircle_list_node_t *scircle_list_node_reverse(scircle_list_node_t *head);
inline static dcircle_list_node_t *dcircle_list_node_reverse(dcircle_list_node_t *head);

#endif