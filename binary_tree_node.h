#ifndef __BINARY_TREE_NODE_H__
#define __BINARY_TREE_NODE_H__

typedef struct binary_tree_node {
    void *data;
    struct binary_tree_node *parent;
    struct binary_tree_node *left;
    struct binary_tree_node *right;
} binary_tree_node_t;

static binary_tree_node_t *binary_tree_node_alloc(void *data);
static void binary_tree_node_clear(binary_tree_node_t *root, void(*dfree)(void *));
static void binary_tree_node_in_order(binary_tree_node_t *root, void(*cb)(binary_tree_node_t *, void *), void *arg);
static void binary_tree_node_pre_order(binary_tree_node_t *root, void(*cb)(binary_tree_node_t *, void *), void *arg);
static void binary_tree_node_post_order(binary_tree_node_t *root, void(*cb)(binary_tree_node_t *, void *), void *arg);
static int  binary_tree_node_size(binary_tree_node_t *root);
static binary_tree_node_t *binary_tree_node_max(binary_tree_node_t *root);
static binary_tree_node_t *binary_tree_node_min(binary_tree_node_t *root);
static binary_tree_node_t *binary_tree_node_pre(binary_tree_node_t *root);
static binary_tree_node_t *binary_tree_node_post(binary_tree_node_t *root);
static binary_tree_node_t *binary_tree_node_at(binary_tree_node_t *root, int n);
static binary_tree_node_t *binary_tree_node_find(binary_tree_node_t *root, void *data, int(*cmp)(void *, void *));
static void binary_tree_node_push(binary_tree_node_t **root, binary_tree_node_t *node, int(*cmp)(void *, void *));
static binary_tree_node_t *binary_tree_node_pop(binary_tree_node_t **root, binary_tree_node_t *node);
static binary_tree_node_t *binary_tree_node_pop_front(binary_tree_node_t **root);
static binary_tree_node_t *binary_tree_node_pop_back(binary_tree_node_t **root);

#endif