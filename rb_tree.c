#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "stack.h"
#include "rb_tree.h"

#define RED   0
#define BLACK 1

typedef struct rb_tree_node {
    void *data;
    unsigned char color;
    struct rb_tree_node *left;
    struct rb_tree_node *right;
    struct rb_tree_node *parent;
} rb_tree_node_t;

typedef struct rb_tree {
    struct rb_tree_node *root;
    int(*compare)(void *, void *); /* nagetive: less than; zero: equal; passive: more than */
    size_t size;
} rb_tree_t;

static rb_tree_node_t *__rb_tree_node_alloc(void *data)
{
    rb_tree_node_t *node = (rb_tree_node_t *)malloc(sizeof(rb_tree_node_t));
    if (!node) return NULL;
    node->data = data;
    node->color = RED;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

static void *__rb_tree_node_free(rb_tree_node_t *node)
{
    void *data = node->data;
    free(node);
    return data;
}

static void __rb_tree_node_pre_order_0(rb_tree_node_t *root, void(*cb)(rb_tree_node_t *, void *), void *arg)
{
    if (!root) return;
    cb(root, arg);
    __rb_tree_node_pre_order_0(root->left, cb, arg);
    __rb_tree_node_pre_order_0(root->right, cb, arg);
}

static void __rb_tree_node_pre_order_1(rb_tree_node_t *root, void(*cb)(rb_tree_node_t *, void *), void *arg)
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

static void __rb_tree_node_pre_order_2(rb_tree_node_t *root, void(*cb)(rb_tree_node_t *, void *), void *arg)
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

static void __rb_tree_node_in_order_0(rb_tree_node_t *root, void(*cb)(rb_tree_node_t *, void *), void *arg)
{
    if (!root) return;
    __rb_tree_node_in_order_0(root->left, cb, arg);
    cb(root, arg);
    __rb_tree_node_in_order_0(root->right, cb, arg);
}

static void __rb_tree_node_in_order_1(rb_tree_node_t *root, void(*cb)(rb_tree_node_t *, void *), void *arg)
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

static void __rb_tree_node_in_order_2(rb_tree_node_t *root, void(*cb)(rb_tree_node_t *, void *), void *arg)
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

static void __rb_tree_node_post_order_0(rb_tree_node_t *root, void(*cb)(rb_tree_node_t *, void *), void *arg)
{
    if (!root) return;
    __rb_tree_node_post_order_0(root->left, cb, arg);
    __rb_tree_node_post_order_0(root->right, cb, arg);
    cb(root, arg);
}

static void __rb_tree_node_post_order_1(rb_tree_node_t *root, void(*cb)(rb_tree_node_t *, void *), void *arg)
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

static void __rb_tree_node_reverse(rb_tree_node_t *n1, rb_tree_node_t *n2)
{
    if (n1 == n2) return;
    rb_tree_node_t *x = n1;
    rb_tree_node_t *y = n1->right;
    do {
        rb_tree_node_t *tmp = y->right;
        y->right = x;
        x = y;
        y = tmp;
    } while (x != n2);
}

static void __rb_tree_node_reverse_cb(rb_tree_node_t *n1, rb_tree_node_t *n2, void(*cb)(rb_tree_node_t *, void *), void *arg)
{
    __rb_tree_node_reverse(n1, n2);
    rb_tree_node_t *node = n2;
    while (node != n1) {
        cb(node, arg);
        node = node->right;
    }
    cb(node, arg);
    __rb_tree_node_reverse(n2, n1);
}

static void __rb_tree_node_post_order_2(rb_tree_node_t *root, void(*cb)(rb_tree_node_t *, void *), void *arg)
{
    if (!root) return;
    rb_tree_node_t *pre, *node, *dummy;
    dummy = __rb_tree_node_alloc(NULL);
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
        __rb_tree_node_reverse_cb(node->left, pre, cb, arg);
        pre->right = NULL;
        node = node->right;
    }
    __rb_tree_node_free(dummy);
}

static rb_tree_node_t *__rb_tree_node_max(rb_tree_node_t *node)
{
    while (node && node->right) node = node->right;
    return node;
}

static rb_tree_node_t *__rb_tree_node_min(rb_tree_node_t *node)
{
    while (node && node->left) node = node->left;
    return node;
}

static rb_tree_node_t *__rb_tree_node_pre(rb_tree_node_t *node)
{
    if (!node) return NULL;
    if (node->left) return __rb_tree_node_max(node->left);
    rb_tree_node_t *pre = node->parent;
    while (pre && pre->left == node) {
        node = pre;
        pre = pre->parent;
    }
    return pre;
}

static rb_tree_node_t *__rb_tree_node_post(rb_tree_node_t *node)
{
    if (!node) return NULL;
    if (node->right) return __rb_tree_node_min(node->right);
    rb_tree_node_t *post = node->parent;
    while (post && post->right == node) {
        node = post;
        post = post->parent;
    }
    return post;
}

static rb_tree_node_t *__rb_tree_node_find(rb_tree_node_t *root, void *data, rb_tree_compare_t compare)
{
    assert(compare);
    if (!root) return NULL;
    rb_tree_node_t *node;
    for (node = root; node; ) {
        int ret = compare(data, node->data);
        if (ret == 0) return node;
        else if (ret > 0) node = node->right;
        else node = node->left;
    }
    return NULL;
}

static void __rb_tree_node_left_rotate(rb_tree_node_t **root, rb_tree_node_t *x)
{
    rb_tree_node_t *y;
    y = x->right;
    x->right = y->left;
    if (y->left) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (!x->parent) {
        *root = y;
    } else if (x->parent->left == x) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

static void __rb_tree_node_right_rotate(rb_tree_node_t **root, rb_tree_node_t *y)
{
    rb_tree_node_t *x;
    x = y->left;
    y->left = x->right;
    if (x->right) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (!y->parent) {
        *root = x;
    } else if (y->parent->right == y) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

static rb_tree_node_t *__rb_tree_insert_fixup_left(rb_tree_node_t **root, rb_tree_node_t *node)
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
        __rb_tree_node_left_rotate(root, node);
    }
    node->parent->color = BLACK;
    node->parent->parent->color = RED;
    __rb_tree_node_right_rotate(root, node->parent->parent);
    return node;
}

static rb_tree_node_t *__rb_tree_insert_fixup_right(rb_tree_node_t **root, rb_tree_node_t *node)
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
        __rb_tree_node_right_rotate(root, node);
    }
    node->parent->color = BLACK;
    node->parent->parent->color = RED;
    __rb_tree_node_left_rotate(root, node->parent->parent);
    return node;
}

static void __rb_tree_insert_fixup(rb_tree_node_t **root, rb_tree_node_t *node)
{
    rb_tree_node_t*(*fixup)(rb_tree_node_t **, rb_tree_node_t *);
    while (node->parent && node->parent->color == RED) {
        fixup = (node->parent == node->parent->parent->left)
            ? __rb_tree_insert_fixup_left
            : __rb_tree_insert_fixup_right;
        node = fixup(root, node);
    }
    (*root)->color = BLACK;
}

static void __rb_tree_insert(rb_tree_t *tree, rb_tree_node_t *node)
{
    assert(tree && tree->compare);
    rb_tree_node_t *x, *parent = NULL;
    for (x = tree->root; x; ) {
        parent = x;
        x = tree->compare(node->data, x->data) < 0 ? x->left : x->right;
    }
    node->parent = parent;
    if (!parent) {
        tree->root = node;
    } else if (tree->compare(node->data, parent->data) < 0) {
        parent->left = node;
    } else {
        parent->right = node;
    }
    __rb_tree_insert_fixup(&tree->root, node);
}

static int __rb_tree_delete_fixup_left(rb_tree_node_t **root, rb_tree_node_t **node, rb_tree_node_t **parent)
{
    rb_tree_node_t *temp = (*parent)->right;
    if (temp && temp->color == RED) {
        temp->color = BLACK;
        (*parent)->color = RED;
        __rb_tree_node_left_rotate(root, (*parent));
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
        __rb_tree_node_right_rotate(root, temp);
        temp = (*parent)->right;
    }
    temp->color = (*parent)->color;
    (*parent)->color = BLACK;
    temp->right->color = BLACK;
    __rb_tree_node_left_rotate(root, (*parent));
    (*node) = *root;
    return 1;
}

static int __rb_tree_delete_fixup_right(rb_tree_node_t **root, rb_tree_node_t **node, rb_tree_node_t **parent)
{
    rb_tree_node_t *temp = (*parent)->left;
    if (temp->color == RED) {
        temp->color = BLACK;
        (*parent)->color = RED;
        __rb_tree_node_right_rotate(root, (*parent));
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
        __rb_tree_node_left_rotate(root, temp);
        temp = (*parent)->left;
    }
    temp->color = (*parent)->color;
    (*parent)->color = BLACK;
    temp->left->color = BLACK;
    __rb_tree_node_right_rotate(root, (*parent));
    (*node) = *root;
    return 1;
}

static void __rb_tree_delete_fixup(rb_tree_node_t **root, rb_tree_node_t *node, rb_tree_node_t *parent)
{
    int(*fixup)(rb_tree_node_t **, rb_tree_node_t **, rb_tree_node_t **);
    while ((!node || node->color == BLACK) && node != *root) {
        fixup = (parent->left == node)
            ? __rb_tree_delete_fixup_left
            : __rb_tree_delete_fixup_right;
        if (fixup(root, &node, &parent)) break;
    }
    if (node) {
        node->color = BLACK;
    }
}

static void __rb_tree_delete(rb_tree_t *tree, rb_tree_node_t *node)
{
    int color;
    rb_tree_node_t *replace, *parent, *child;

    if (node->left && node->right) {
        replace = __rb_tree_node_min(node->right);
        if (!node->parent) {
            tree->root = replace;
        } else if (node->parent->left == node) {
            node->parent->left = replace;
        } else {
            node->parent->right = replace;
        }
        child = replace->right;
        parent = replace->parent;
        color = replace->color;
        if (parent != node) {
            if (child) child->parent = parent;
            parent->left = child;
            replace->right = node->right;
            node->right->parent = replace;
        } else {
            parent = replace;
        }
        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;
        if (color == BLACK) {
            __rb_tree_delete_fixup(&tree->root, child, parent);
        }
    } else {
        child = node->left ? node->left : node->right;
        parent = node->parent;
        color = node->color;
        if (child) {
            child->parent = parent;
        }
        if (!parent) {
            tree->root = child;
        } else if (parent->left == node) {
            parent->left = child;
        } else {
            parent->right = child;
        }
        if (color == BLACK) {
            __rb_tree_delete_fixup(&tree->root, child, parent);
        }
    }
}

/**
 * API implementation
 */

rb_tree_t *rb_tree_alloc(rb_tree_compare_t compare)
{
    rb_tree_t *tree = (rb_tree_t *)malloc(sizeof(rb_tree_t));
    if (!tree) return NULL;
    tree->root = NULL;
    tree->size = 0;
    tree->compare = compare;
    return tree;
}

static void __rb_tree_node_free_cb(rb_tree_node_t *node, void *arg)
{
    rb_tree_free_t freecb = (rb_tree_free_t)arg;
    void *data = __rb_tree_node_free(node);
    if (freecb) freecb(data);
}

void rb_tree_clear(rb_tree_t *tree, rb_tree_free_t freecb)
{
    if (!tree) return;
    __rb_tree_node_post_order_0(tree->root, __rb_tree_node_free_cb, freecb);
    tree->root = NULL;
    tree->size = 0;
}

void rb_tree_free(rb_tree_t *tree, rb_tree_free_t freecb)
{
    if (!tree) return;
    __rb_tree_node_post_order_0(tree->root, __rb_tree_node_free_cb, freecb);
    free(tree);
}

size_t rb_tree_size(rb_tree_t *tree)
{
    return tree->size;
}

void rb_tree_push(rb_tree_t *tree, void *data)
{
    if (!tree) return;
    rb_tree_node_t *node = __rb_tree_node_alloc(data);
    __rb_tree_insert(tree, node);
    tree->size++;
}

inline static void *__rb_tree_pop_node(rb_tree_t *tree, rb_tree_node_t *node)
{
    if (!tree) return NULL;
    if (!tree->root) return NULL;
    if (!node) return NULL;
    tree->size--;
    __rb_tree_delete(tree, node);
    return __rb_tree_node_free(node);
}

void *rb_tree_pop(rb_tree_t *tree, void *data)
{
    rb_tree_node_t *node;
    node = __rb_tree_node_find(tree->root, data, tree->compare);
    return __rb_tree_pop_node(tree, node);
}

void *rb_tree_pop_front(rb_tree_t *tree)
{
    rb_tree_node_t *node;
    node = __rb_tree_node_min(tree->root);
    return __rb_tree_pop_node(tree, node);
}

void *rb_tree_pop_back(rb_tree_t *tree)
{
    rb_tree_node_t *node;
    node = __rb_tree_node_max(tree->root);
    return __rb_tree_pop_node(tree, node);
}

void *rb_tree_front(rb_tree_t *tree)
{
    if (!tree) return NULL;
    if (!tree->root) return NULL;
    rb_tree_node_t *node = __rb_tree_node_min(tree->root);
    return node->data;
}

void *rb_tree_back(rb_tree_t *tree)
{
    if (!tree) return NULL;
    if (!tree->root) return NULL;
    rb_tree_node_t *node = __rb_tree_node_max(tree->root);
    return node->data;
}

void *rb_tree_find(rb_tree_t *tree, void *data)
{
    rb_tree_node_t *node;
    node = __rb_tree_node_find(tree->root, data, tree->compare);
    return node->data;
}

struct __rb_tree_foreach_user_cb {
    void(*func)(void *, void *);
    void *arg;
};

static void __rb_tree_foreach_cb(rb_tree_node_t *node, void *arg)
{
    struct __rb_tree_foreach_user_cb *ucb;
    ucb = (struct __rb_tree_foreach_user_cb *)arg;
    printf("value: %d, color: %d\n", *(int *)node->data, node->color);
    if (ucb) ucb->func(node->data, ucb->arg);
}

void rb_tree_foreach(rb_tree_t *tree, void(*cb)(void *, void *), void *arg)
{
    struct __rb_tree_foreach_user_cb ucb;
    ucb.func = cb;
    ucb.arg = arg;
    __rb_tree_node_in_order_2(tree->root, __rb_tree_foreach_cb, &ucb);
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
static void rb_tree_print(void *data, void *arg)
{
    printf("data = %d\n", *(int *)data);
}
void rb_tree_test()
{
    rb_tree_t *tree = rb_tree_alloc(rb_tree_int_less_than);
    int key[] = { 2,5,6,4,3,1,8,9,7,0 };

    rb_tree_push(tree, &key[0]);
    assert(*(int *)rb_tree_front(tree) == key[0]);
    assert(*(int *)rb_tree_back(tree)  == key[0]);

    rb_tree_push(tree, &key[1]);
    assert(*(int *)rb_tree_front(tree) == key[0]);
    assert(*(int *)rb_tree_back(tree)  == key[1]);

    rb_tree_push(tree, &key[2]);
    assert(*(int *)rb_tree_front(tree) == key[0]);
    assert(*(int *)rb_tree_back(tree)  == key[2]);

    rb_tree_push(tree, &key[3]);
    assert(*(int *)rb_tree_front(tree) == key[0]);
    assert(*(int *)rb_tree_back(tree)  == key[2]);

    rb_tree_push(tree, &key[4]);
    assert(*(int *)rb_tree_front(tree) == key[0]);
    assert(*(int *)rb_tree_back(tree)  == key[2]);

    rb_tree_push(tree, &key[5]);
    assert(*(int *)rb_tree_front(tree) == key[5]);
    assert(*(int *)rb_tree_back(tree)  == key[2]);

    rb_tree_push(tree, &key[6]);
    assert(*(int *)rb_tree_front(tree) == key[5]);
    assert(*(int *)rb_tree_back(tree)  == key[6]);

    rb_tree_push(tree, &key[7]);
    assert(*(int *)rb_tree_front(tree) == key[5]);
    assert(*(int *)rb_tree_back(tree)  == key[7]);

    rb_tree_push(tree, &key[8]);
    assert(*(int *)rb_tree_front(tree) == key[5]);
    assert(*(int *)rb_tree_back(tree)  == key[7]);

    rb_tree_push(tree, &key[9]);
    assert(*(int *)rb_tree_front(tree) == key[9]);
    assert(*(int *)rb_tree_back(tree)  == key[7]);

    rb_tree_foreach(tree, rb_tree_print, NULL);
    printf("\n");

    rb_tree_pop(tree, &key[5]);
    assert(*(int *)rb_tree_front(tree) == key[9]);
    assert(*(int *)rb_tree_back(tree)  == key[7]);

    rb_tree_pop(tree, &key[3]);
    assert(*(int *)rb_tree_front(tree) == key[9]);
    assert(*(int *)rb_tree_back(tree)  == key[7]);

    rb_tree_pop(tree, &key[5]);
    rb_tree_pop(tree, &key[7]);
    assert(*(int *)rb_tree_front(tree) == key[9]);
    assert(*(int *)rb_tree_back(tree)  == key[6]);

    rb_tree_foreach(tree, rb_tree_print, NULL);
    printf("\n");

    for (int i = 0; i < 7; i++) {
        rb_tree_pop_front(tree);
        assert(rb_tree_size(tree) == 6 - i);
    }

    rb_tree_free(tree, NULL);
}
#endif