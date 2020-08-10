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
    int     key_sizeof_type;
    int     value_sizeof_type;
    void* (*key_alloc)(void *);
    void* (*value_alloc)(void *);
    void  (*key_free)(void *);
    void  (*value_free)(void *);
} rb_tree_t;

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
        if (!pre->right) {
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
    rb_tree_node_t *pre, *node;
    rb_tree_node_t *dummy = (rb_tree_node_t *)malloc(sizeof(rb_tree_node_t));
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
    free(dummy);
}

static rb_tree_node_t *rb_tree_node_search(rb_tree_node_t *root, void *key, int(*compare)(void *, void *))
{
    if (!root) return NULL;
    rb_tree_node_t *node;
    for (node = root;  node; ) {
        int ret = compare(key, node->key);
        if (ret == 0) return node;
        else if (ret > 0) node = node->right;
        else node = node->left;
    }
    return NULL;
}

/* 
 *
 *      px                              px
 *     /                               /
 *    x                               y                
 *   /  \      --(left rotate)-->    / \                #
 *  lx   y                          x  ry     
 *     /   \                       /  \
 *    ly   ry                     lx  ly  
 *
 *
 */
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

/* 
 *
 *            py                               py
 *           /                                /
 *          y                                x                  
 *         /  \    --(right rotate)-->      /  \                     #
 *        x   ry                           lx   y  
 *       / \                                   / \                   #
 *      lx  rx                                rx  ry
 * 
 */
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

static void rb_tree_insert_fixup_left(rb_tree_t *tree, rb_tree_node_t **node)
{
    rb_tree_node_t *temp = (*node)->parent->parent->right;
    if (temp->color == RED) {
        (*node)->parent->color = BLACK;
        (*node)->parent->parent->color = RED;
        temp->color = BLACK;
        (*node) = (*node)->parent->parent;
    } else if ((*node) == (*node)->parent->right) {
        (*node) = (*node)->parent;
        rb_tree_node_left_rotate(tree, (*node));
    } else {
        (*node)->parent->color = BLACK;
        (*node)->parent->parent->color = RED;
        rb_tree_node_right_rotate(tree, (*node)->parent->parent);
    }
}

static void rb_tree_insert_fixup_right(rb_tree_t *tree, rb_tree_node_t **node)
{
    rb_tree_node_t *temp = (*node)->parent->parent->left;
    if (temp->color == RED) {
        (*node)->parent->color = BLACK;
        (*node)->parent->parent->color = RED;
        temp->color = BLACK;
        (*node) = (*node)->parent->parent;
    } else if ((*node) == (*node)->parent->left) {
        (*node) = (*node)->parent;
        rb_tree_node_right_rotate(tree, (*node));
    } else {
        (*node)->parent->color = BLACK;
        (*node)->parent->parent->color = RED;
        rb_tree_node_left_rotate(tree, (*node)->parent->parent);
    }
}

static void rb_tree_insert_fixup(rb_tree_t *tree, rb_tree_node_t *node)
{
    rb_tree_node_t *temp;
    while (node != tree->root && node->parent->color == RED) {
        if (node->parent == node->parent->parent->left) {
            rb_tree_insert_fixup_left(tree, &node);
        } else {
            rb_tree_insert_fixup_right(tree, &node);
        }
    }
    tree->root->color = BLACK;
}

static void rb_tree_insert(rb_tree_t *tree, rb_tree_node_t *node)
{
    rb_tree_node_t *x, *parent = NULL;
    for (x = tree->root; x; ) {
        parent = x;
        x = tree->compare(node->key, x->key) ? x->left : x->right;
    }
    node->parent = parent;
    if (!parent) {
        tree->root = node;
    } else if (tree->compare(node->key, parent->key)) {
        parent->left = node;
    } else {
        parent->right = node;
    }
    rb_tree_insert_fixup(tree, node);
}

static int rb_tree_delete_fixup_left(rb_tree_t *tree, rb_tree_node_t **node, rb_tree_node_t **parent)
{
    rb_tree_node_t *temp = (*parent)->right;
    if (temp->color == RED) {
        // Case 1: x的兄弟w是红色的
        temp->color = BLACK;
        (*parent)->color = RED;
        rb_tree_node_left_rotate(tree, (*parent));
        temp = (*parent)->right;
    }
    if ((!temp->left || temp->left->color == BLACK) &&
        (!temp->right || temp->right->color == BLACK)) {
        // Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的
        temp->color = RED;
        (*node) = (*parent);
        (*parent) = (*node)->parent;
    } else {
        if (!temp->right || temp->right->color == BLACK) {
            // Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。
            temp->left->color = BLACK;
            temp->color = RED;
            rb_tree_node_right_rotate(tree, temp);
            temp = (*parent)->right;
        }
        // Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
        temp->color = (*parent)->color;
        (*parent)->color = BLACK;
        temp->right->color = BLACK;
        rb_tree_node_left_rotate(tree, (*parent));
        (*node) = tree->root;
        return 1;
    }
    return 0;
}

static int rb_tree_delete_fixup_right(rb_tree_t *tree, rb_tree_node_t **node, rb_tree_node_t **parent)
{
    rb_tree_node_t *temp = (*parent)->left;
    if (temp->color == RED) {
        // Case 1: x的兄弟w是红色的  
        temp->color = BLACK;
        (*parent)->color = RED;
        rb_tree_node_right_rotate(tree, (*parent));
        temp = (*parent)->left;
    }
    if ((!temp->left || temp->left->color == BLACK) &&
        (!temp->right || temp->right->color == BLACK)) {
        // Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的
        temp->color = RED;
        (*node) = (*parent);
        (*parent) = (*node)->parent;
    } else {
        if (!temp->left || temp->left->color == BLACK) {
            // Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。
            temp->right->color = BLACK;
            temp->color = RED;
            rb_tree_node_left_rotate(tree, temp);
            temp = (*parent)->left;
        }
        // Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
        temp->color = (*parent)->color;
        (*parent)->color = BLACK;
        temp->left->color = BLACK;
        rb_tree_node_right_rotate(tree, (*parent));
        (*node) = tree->root;
        return 1;
    }
    return 0;
}

static void rb_tree_delete_fixup(rb_tree_t *tree, rb_tree_node_t *node, rb_tree_node_t *parent)
{
    while ((!node || node->color == BLACK) && node != tree->root) {
        if (parent->left == node) {
            if (rb_tree_delete_fixup_left(tree, &node, &parent)) break;
        } else {
            if (rb_tree_delete_fixup_right(tree, &node, &parent)) break;
        }
    }

    if (node) {
        node->color = BLACK;
    }
}

static void rb_tree_delete(rb_tree_t *tree, rb_tree_node_t *node)
{
    int color;
    rb_tree_node_t *child, *parent;

    // 被删除节点的"左右孩子都不为空"的情况。
    if (node->left && node->right) {
        // 被删节点的后继节点。(称为"取代节点")
        // 用它来取代"被删节点"的位置，然后再将"被删节点"去掉。
        rb_tree_node_t *replace = node;

        // 获取后继节点
        replace = replace->right;
        while (replace->left) {
            replace = replace->left;
        }

        // "node节点"不是根节点(只有根节点不存在父节点)
        if (!node->parent) {
            // "node节点"是根节点，更新根节点。
            tree->root = replace; 
        } else if (node->parent->left == node) {
            node->parent->left = replace;
        } else {
            node->parent->right = replace;
        }

        // child是"取代节点"的右孩子，也是需要"调整的节点"。
        // "取代节点"肯定不存在左孩子！因为它是一个后继节点。
        child = replace->right;
        parent = replace->parent;
        // 保存"取代节点"的颜色
        color = replace->color;

        // "被删除节点"是"它的后继节点的父节点"
        if (parent == node) {
            parent = replace;
        } else {
            // child不为空
            if (child) child->parent = parent;
            parent->left = child;
            replace->right = node->right;
            node->right->parent = replace;
        }

        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;
        
    } else {

        child = node->left ? node->left : node->right;

        parent = node->parent;
        // 保存"取代节点"的颜色
        color = node->color;

        if (child) {
            child->parent = parent;
        }

        // "node节点"不是根节点
        if (!parent) {
            tree->root = child;
        } else if (parent->left == node) {
            parent->left = child;
        } else {
            parent->right = child;
        }
    }

    if (color == BLACK) {
        rb_tree_delete_fixup(tree, child, parent);
    }
    free(node);
}

static rb_tree_node_t *rb_tree_node_alloc(void *key, void *value)
{
    rb_tree_node_t *node = (rb_tree_node_t *)malloc(sizeof(rb_tree_node_t));
    if (!node) return NULL;
    node->color  = RED;
    node->key    = key;
    node->value  = value;
    node->left   = NULL;
    node->right  = NULL;
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
    rb_tree_node_t *node = rb_tree_node_search(tree->root, key, tree->compare);
    rb_tree_delete(tree, node);
}

void *rb_tree_find(rb_tree_t *tree, void *key)
{
    rb_tree_node_t *node = rb_tree_node_search(tree->root, key, tree->compare);
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
    return *(int *)i1 < *(int *)i2;
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

    // rb_tree_pop(tree, &key[0]);

    rb_tree_free(tree);
}
#endif