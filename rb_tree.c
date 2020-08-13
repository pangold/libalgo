#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "stack.h"
#include "rb_tree.h"

#define RED   0
#define BLACK 1

typedef struct rb_tree_node {
    void *key;
    void *value;
    unsigned char color;
    struct rb_tree_node *left;
    struct rb_tree_node *right;
    struct rb_tree_node *parent;
} rb_tree_node_t;

typedef struct rb_tree {
    struct rb_tree_node *root;
    struct rb_tree_node *nil;
    int   (*compare)(void *, void *);
    void* (*key_alloc)(void *);
    void* (*value_alloc)(void *);
    void  (*key_free)(void *);
    void  (*value_free)(void *);
} rb_tree_t;

static rb_tree_node_t *rb_tree_node_alloc(void *key, void *value)
{
    rb_tree_node_t *node = (rb_tree_node_t *)malloc(sizeof(rb_tree_node_t));
    if (!node) return NULL;
    node->color = RED;
    node->key = key;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

static void rb_tree_node_free(rb_tree_node_t *node, void *arg)
{
    rb_tree_t *tree = (rb_tree_t *)arg;
    if (tree && tree->key_free && node->key) tree->key_free(node->key);
    if (tree && tree->value_free && node->value) tree->value_free(node->value);
    free(node);
}

static void rb_tree_node_pre_order_0(rb_tree_node_t *root, void(*cb)(rb_tree_node_t *, void *), void *arg)
{
    if (!root) return;
    cb(root, arg);
    rb_tree_node_pre_order_0(root->left, cb, arg);
    rb_tree_node_pre_order_0(root->right, cb, arg);
}

static void rb_tree_node_pre_order_1(rb_tree_node_t *root, void(*cb)(rb_tree_node_t *, void *), void *arg)
{
    if (!root) return;
    rb_tree_node_t *node;
    stack_t *stack = stack_alloc();
    stack_push(stack, root);
    while (stack_size(stack) != 0) {
        node = (rb_tree_node_t *)stack_pop(stack);
        cb(node, arg);
        if (node->right) stack_push(stack, node->right);
        if (node->left) stack_push(stack, node->left);
    }
    stack_free(stack, NULL);
}

static void rb_tree_node_pre_order_2(rb_tree_node_t *root, void(*cb)(rb_tree_node_t *, void *), void *arg)
{
    if (!root) return;
    rb_tree_node_t *pre, *node;
    for (node = root; node; ) {
        if (!node->left) {
            cb(node, arg);
            node = node->right;
            continue;
        }
        pre = node->left;
        while (pre->right && pre->right != node) pre = pre->right;
        if (!pre->right) {
            pre->right = node;
            cb(node, arg);
            node = node->left;
            continue;
        }
        pre->right = NULL;
        node = node->right;
    }
}

static void rb_tree_node_in_order_0(rb_tree_node_t *root, void(*cb)(rb_tree_node_t *, void *), void *arg)
{
    if (!root) return;
    rb_tree_node_in_order_0(root->left, cb, arg);
    cb(root, arg);
    rb_tree_node_in_order_0(root->right, cb, arg);
}

static void rb_tree_node_in_order_1(rb_tree_node_t *root, void(*cb)(rb_tree_node_t *, void *), void *arg)
{
    if (!root) return;
    stack_t *stack = stack_alloc();
    rb_tree_node_t *node = root;
    while (node || stack_size(stack) != 0) {
        if (!node) {
            node = (rb_tree_node_t *)stack_pop(stack);
            cb(node, arg);
            node = node->right;
            continue;
        }
        stack_push(stack, node);
        node = node->left;
    }
    stack_free(stack, NULL);
}

static void rb_tree_node_in_order_2(rb_tree_node_t *root, void(*cb)(rb_tree_node_t *, void *), void *arg)
{
    if (!root) return;
    rb_tree_node_t *pre, *node;
    for (node = root; node; ) {
        if (!node->left) {
            cb(node, arg);
            node = node->right;
            continue;
        }
        pre = node->left;
        while (pre->right && pre->right != node) pre = pre->right;
        if (pre->right) {
            pre->right = NULL;
            cb(node, arg);
            node = node->right;
            continue;
        }
        pre->right = node;
        node = node->left;
    }
}

static void rb_tree_node_post_order_0(rb_tree_node_t *root, void(*cb)(rb_tree_node_t *, void *), void *arg)
{
    if (!root) return;
    rb_tree_node_post_order_0(root->left, cb, arg);
    rb_tree_node_post_order_0(root->right, cb, arg);
    cb(root, arg);
}

static void rb_tree_node_post_order_1(rb_tree_node_t *root, void(*cb)(rb_tree_node_t *, void *), void *arg)
{
    if (!root) return;
    rb_tree_node_t *pre, *node;
    stack_t *stack = stack_alloc();
    stack_t *stack2 = stack_alloc();
    stack_push(stack, root);
    while (stack_size(stack) != 0) {
        node = (rb_tree_node_t *)stack_pop(stack);
        stack_push(stack2, node);
        if (node->left) stack_push(stack, node->left);
        if (node->right) stack_push(stack, node->right);
    }
    while (stack_size(stack2) != 0) {
        node = (rb_tree_node_t *)stack_pop(stack2);
        cb(node, arg);
    }
}

static void rb_tree_node_reverse(rb_tree_node_t *n1, rb_tree_node_t *n2)
{
    if (n1 == n2) return;
    rb_tree_node_t *x = n1;
    rb_tree_node_t *y = n1->right;
    while (1) {
        rb_tree_node_t *tmp = y->right;
        y->right = x;
        x = y;
        y = tmp;
        if (x == n2) break;
    }
}

static void rb_tree_node_reverse_cb(rb_tree_node_t *n1, rb_tree_node_t *n2, void(*cb)(rb_tree_node_t *, void *), void *arg)
{
    rb_tree_node_reverse(n1, n2);
    rb_tree_node_t *node = n2;
    while (1) {
        cb(node, arg);
        if (node == n1) break;
        node = node->right;
    }
    rb_tree_node_reverse(n2, n1);
}

static void rb_tree_node_post_order_2(rb_tree_node_t *root, void(*cb)(rb_tree_node_t *, void *), void *arg)
{
    if (!root) return;
    rb_tree_node_t *pre, *node, *dummy;
    dummy = rb_tree_node_alloc(NULL, NULL);
    dummy->left = root;
    for (node = dummy; node; ) {
        if (!node->left) {
            node = node->right;
            continue;
        }
        pre = node->left;
        while (pre->right && pre->right != node) pre = pre->right;
        if (!pre->right) {
            pre->right = node;
            node = node->left;
            continue;
        }
        rb_tree_node_reverse_cb(node->left, pre, cb, arg);
        pre->right = NULL;
        node = node->right;
    }
    rb_tree_node_free(dummy, NULL);
}

static rb_tree_node_t *rb_tree_node_max(rb_tree_node_t *node)
{
    while (node && node->right) node = node->right;
    return node;
}

static rb_tree_node_t *rb_tree_node_min(rb_tree_node_t *node)
{
    while (node && node->left) node = node->left;
    return node;
}

static rb_tree_node_t *rb_tree_node_pre(rb_tree_node_t *node)
{
    if (!node) return NULL;
    if (node->left) return rb_tree_node_max(node->left);
    rb_tree_node_t *pre = node->parent;
    while (pre && pre->left == node) {
        node = pre;
        pre = pre->parent;
    }
    return pre;
}

static rb_tree_node_t *rb_tree_node_post(rb_tree_node_t *node)
{
    if (!node) return NULL;
    if (node->right) return rb_tree_node_min(node->right);
    rb_tree_node_t *post = node->parent;
    while (post && post->right == node) {
        node = post;
        post = post->parent;
    }
    return post;
}

static rb_tree_node_t *rb_tree_node_find_by_key(rb_tree_node_t *root, void *key, int(*compare)(void *, void *))
{
    if (!root) return NULL;
    rb_tree_node_t *node;
    for (node = root; node; ) {
        int ret = compare(key, node->key);
        if (ret == 0) return node;
        else if (ret > 0) node = node->right;
        else node = node->left;
    }
    return NULL;
}

static void rb_tree_node_left_rotate(rb_tree_t *tree, rb_tree_node_t *x)
{
    rb_tree_node_t *y;
    y = x->right;
    x->right = y->left;
    if (y->left) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (!x->parent) {
        tree->root = y;
    } else if (x->parent->left == x) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

static void rb_tree_node_right_rotate(rb_tree_t *tree, rb_tree_node_t *y)
{
    rb_tree_node_t *x;
    x = y->left;
    y->left = x->right;
    if (x->right) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (!y->parent) {
        tree->root = x;
    } else if (y->parent->right == y) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

static rb_tree_node_t *rb_tree_insert_fixup_left(rb_tree_t *tree, rb_tree_node_t *node)
{
    rb_tree_node_t *temp = node->parent->parent->right;
    if (temp && temp->color == RED) {
        temp->color = BLACK;
        node->parent->color = BLACK;
        node->parent->parent->color = RED;
        return node->parent->parent;
    }
    if (node == node->parent->right) {
        node = node->parent;
        rb_tree_node_left_rotate(tree, node);
    }
    node->parent->color = BLACK;
    node->parent->parent->color = RED;
    rb_tree_node_right_rotate(tree, node->parent->parent);
    return node;
}

static rb_tree_node_t *rb_tree_insert_fixup_right(rb_tree_t *tree, rb_tree_node_t *node)
{
    rb_tree_node_t *temp = node->parent->parent->left;
    if (temp && temp->color == RED) {
        temp->color = BLACK;
        node->parent->color = BLACK;
        node->parent->parent->color = RED;
        return node->parent->parent;
    }
    if (node == node->parent->left) {
        node = node->parent;
        rb_tree_node_right_rotate(tree, node);
    }
    node->parent->color = BLACK;
    node->parent->parent->color = RED;
    rb_tree_node_left_rotate(tree, node->parent->parent);
    return node;
}

static void rb_tree_insert_fixup(rb_tree_t *tree, rb_tree_node_t *node)
{
    rb_tree_node_t*(*fixup)(rb_tree_t *, rb_tree_node_t *);
    while (node->parent && node->parent->color == RED) {
        fixup = (node->parent == node->parent->parent->left)
            ? rb_tree_insert_fixup_left
            : rb_tree_insert_fixup_right;
        node = fixup(tree, node);
    }
    tree->root->color = BLACK;
}

static void rb_tree_insert(rb_tree_t *tree, rb_tree_node_t *node)
{
    rb_tree_node_t *x, *parent = NULL;
    for (x = tree->root; x; ) {
        parent = x;
        x = tree->compare(node->key, x->key) < 0 ? x->left : x->right;
    }
    node->parent = parent;
    if (!parent) {
        tree->root = node;
    } else if (tree->compare(node->key, parent->key) < 0) {
        parent->left = node;
    } else {
        parent->right = node;
    }
    rb_tree_insert_fixup(tree, node);
}

static int rb_tree_delete_fixup_left(rb_tree_t *tree, rb_tree_node_t **node, rb_tree_node_t **parent)
{
    rb_tree_node_t *temp = (*parent)->right;
    if (temp && temp->color == RED) {
        temp->color = BLACK;
        (*parent)->color = RED;
        rb_tree_node_left_rotate(tree, (*parent));
        temp = (*parent)->right;
    }
    if ((!temp->left || temp->left->color == BLACK) &&
        (!temp->right || temp->right->color == BLACK)) {
        temp->color = RED;
        (*node) = (*parent);
        (*parent) = (*node)->parent;
        return 0;
    }
    if (!temp->right || temp->right->color == BLACK) {
        temp->left->color = BLACK;
        temp->color = RED;
        rb_tree_node_right_rotate(tree, temp);
        temp = (*parent)->right;
    }
    temp->color = (*parent)->color;
    (*parent)->color = BLACK;
    temp->right->color = BLACK;
    rb_tree_node_left_rotate(tree, (*parent));
    (*node) = tree->root;
    return 1;
}

static int rb_tree_delete_fixup_right(rb_tree_t *tree, rb_tree_node_t **node, rb_tree_node_t **parent)
{
    rb_tree_node_t *temp = (*parent)->left;
    if (temp->color == RED) {
        temp->color = BLACK;
        (*parent)->color = RED;
        rb_tree_node_right_rotate(tree, (*parent));
        temp = (*parent)->left;
    }
    if ((!temp->left || temp->left->color == BLACK) &&
        (!temp->right || temp->right->color == BLACK)) {
        temp->color = RED;
        (*node) = (*parent);
        (*parent) = (*node)->parent;
        return 0;
    }
    if (!temp->left || temp->left->color == BLACK) {
        temp->right->color = BLACK;
        temp->color = RED;
        rb_tree_node_left_rotate(tree, temp);
        temp = (*parent)->left;
    }
    temp->color = (*parent)->color;
    (*parent)->color = BLACK;
    temp->left->color = BLACK;
    rb_tree_node_right_rotate(tree, (*parent));
    (*node) = tree->root;
    return 1;
}

static void rb_tree_delete_fixup(rb_tree_t *tree, rb_tree_node_t *node, rb_tree_node_t *parent)
{
    int(*fixup)(rb_tree_t *, rb_tree_node_t **, rb_tree_node_t **);
    while ((!node || node->color == BLACK) && node != tree->root) {
        fixup = (parent->left == node)
            ? rb_tree_delete_fixup_left
            : rb_tree_delete_fixup_right;
        if (fixup(tree, &node, &parent)) break;
    }
    if (node) {
        node->color = BLACK;
    }
}

static void rb_tree_delete(rb_tree_t *tree, rb_tree_node_t *node)
{
    rb_tree_node_t *replace;

    if (node->left && node->right) {
        replace = rb_tree_node_min(node->right);
        if (!node->parent) {
            tree->root = replace; 
        } else if (node->parent->left == node) {
            node->parent->left = replace;
        } else {
            node->parent->right = replace;
        }
        if (replace->parent != node) {
            if (replace->right) replace->right->parent = replace->parent;
            replace->parent->left = replace->right;
            replace->right = node->right;
            node->right->parent = replace;
        } else {
            replace->parent = replace;
        }
        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;
        if (replace->color == BLACK) {
            rb_tree_delete_fixup(tree, replace->right, replace->parent);
        }
    } else {
        replace = node->left ? node->left : node->right;
        if (replace) {
            replace->parent = node->parent;
        }
        if (!node->parent) {
            tree->root = replace;
        } else if (node->parent->left == node) {
            node->parent->left = replace;
        } else {
            node->parent->right = replace;
        }
        if (node->color == BLACK) {
            rb_tree_delete_fixup(tree, replace, node->parent);
        }
    }
}

rb_tree_t *rb_tree_alloc()
{
    rb_tree_t *tree = (rb_tree_t *)malloc(sizeof(rb_tree_t));
    if (!tree) return NULL;
    tree->nil         = (rb_tree_node_alloc(NULL, NULL));
    tree->nil->color  = BLACK;
    tree->root        = NULL;
    tree->compare     = NULL;
    tree->key_alloc   = NULL;
    tree->key_free    = NULL;
    tree->value_alloc = NULL;
    tree->value_free  = NULL;
    return tree;
}

void rb_tree_clear(rb_tree_t *tree)
{
    if (!tree) return;
    rb_tree_node_post_order_0(tree->root, rb_tree_node_free, tree);
    tree->root = NULL;
}

void rb_tree_free(rb_tree_t *tree)
{
    if (!tree) return;
    rb_tree_node_post_order_0(tree->root, rb_tree_node_free, tree);
    free(tree->nil);
    free(tree);
}

void rb_tree_set_key_alloc(rb_tree_t *tree, void*(*alloc)(void *))
{
    tree->key_alloc = alloc;
}

void rb_tree_set_value_alloc(rb_tree_t *tree, void*(*alloc)(void *))
{
    tree->value_alloc = alloc;
}

void rb_tree_set_key_free(rb_tree_t *tree, void(*free)(void *))
{
    tree->key_free = free;
}

void rb_tree_set_value_free(rb_tree_t *tree, void(*free)(void *))
{
    tree->value_free = free;
}

void rb_tree_set_compare(rb_tree_t *tree, int(*compare)(void *, void *))
{
    tree->compare = compare;
}

void rb_tree_push(rb_tree_t *tree, void *key, void *value)
{
    rb_tree_node_t *node = rb_tree_node_alloc(key, value);
    rb_tree_insert(tree, node);
}

void rb_tree_pop(rb_tree_t *tree, void *key)
{
    rb_tree_node_t *node;
    node = rb_tree_node_find_by_key(tree->root, key, tree->compare);
    rb_tree_delete(tree, node);
    rb_tree_node_free(node, tree);
}

void *rb_tree_find(rb_tree_t *tree, void *key)
{
    rb_tree_node_t *node;
    node = rb_tree_node_find_by_key(tree->root, key, tree->compare);
    return node->value;
}

void *rb_tree_at(rb_tree_t *tree, size_t n)
{
    return NULL;
}

struct rb_tree_foreach_user_cb {
    void(*func)(void *, void *, void *);
    void *arg;
};

static void rb_tree_foreach_cb(rb_tree_node_t *node, void *arg)
{
    struct rb_tree_foreach_user_cb *ucb;
    ucb = (struct rb_tree_foreach_user_cb *)arg;
    ucb->func(node->key, node->value, ucb->arg);
}

void rb_tree_foreach(rb_tree_t *tree, void(*cb)(void *, void *, void *), void *arg)
{
    struct rb_tree_foreach_user_cb ucb;
    ucb.func = cb;
    ucb.arg = arg;
    rb_tree_node_in_order_2(tree->root, rb_tree_foreach_cb, &ucb);
}

#if defined(ENABLE_RBTREE_TEST)
static void *rb_tree_kv_int_alloc(void *src)
{
    void *key = malloc(sizeof(int));
    *(int *)key = *(int *)src;
    return key;
}
static void rb_tree_kv_int_free(void *src)
{
    free(src);
}
static int rb_tree_int_less_than(void *i1, void *i2)
{
    return *(int *)i1 - *(int *)i2;
}
static void rb_tree_print(void *key, void *value, void *arg)
{
    printf("key = %d, value = %d\n", *(int *)key, *(int *)value);
}
void rb_tree_test()
{
    rb_tree_t *tree = rb_tree_alloc();
    int key[] = { 2,5,6,4,3,1,8,9,7,0 };
    int value[] = { 22,55,66,44,33,11,88,99,77,10 };

    // rb_tree_set_key_alloc(tree, rb_tree_kv_int_alloc);
    // rb_tree_set_key_free(tree, rb_tree_kv_int_free);
    // rb_tree_set_value_alloc(tree, rb_tree_kv_int_alloc);
    // rb_tree_set_value_free(tree, rb_tree_kv_int_free);
    rb_tree_set_compare(tree, rb_tree_int_less_than);

    rb_tree_push(tree, &key[0], &value[0]);
    rb_tree_push(tree, &key[1], &value[1]);
    rb_tree_push(tree, &key[2], &value[2]);
    rb_tree_push(tree, &key[3], &value[3]);
    rb_tree_push(tree, &key[4], &value[4]);
    rb_tree_push(tree, &key[5], &value[5]);
    rb_tree_push(tree, &key[6], &value[6]);
    rb_tree_push(tree, &key[7], &value[7]);
    rb_tree_push(tree, &key[8], &value[8]);
    rb_tree_push(tree, &key[9], &value[9]);

    rb_tree_foreach(tree, rb_tree_print, NULL);
    printf("\n");

    rb_tree_pop(tree, &key[3]);
    rb_tree_pop(tree, &key[5]);
    rb_tree_pop(tree, &key[7]);
    rb_tree_foreach(tree, rb_tree_print, NULL);
    printf("\n");

    // rb_tree_pop(tree, &key[0]);

    rb_tree_free(tree);
}
#endif