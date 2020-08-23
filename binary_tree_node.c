#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "stack.h"
#include "binary_tree_node.h"

static void binary_tree_node_pre_order_0(binary_tree_node_t *root, void(*cb)(binary_tree_node_t *, void *), void *arg)
{
    if (!root) return;
    cb(root, arg);
    binary_tree_node_pre_order_0(root->left, cb, arg);
    binary_tree_node_pre_order_0(root->right, cb, arg);
}

static void binary_tree_node_pre_order_1(binary_tree_node_t *root, void(*cb)(binary_tree_node_t *, void *), void *arg)
{
    if (!root) return;
    binary_tree_node_t *node;
    algo_stack_t *stack = stack_alloc();
    stack_push(stack, root);
    while (stack_size(stack) != 0) {
        node = (binary_tree_node_t *)stack_pop(stack);
        cb(node, arg);
        if (node->right) stack_push(stack, node->right);
        if (node->left) stack_push(stack, node->left);
    }
    stack_free(stack, NULL);
}

static void binary_tree_node_pre_order_2(binary_tree_node_t *root, void(*cb)(binary_tree_node_t *, void *), void *arg)
{
    if (!root) return;
    binary_tree_node_t *pre, *node;
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

static void binary_tree_node_in_order_0(binary_tree_node_t *root, void(*cb)(binary_tree_node_t *, void *), void *arg)
{
    if (!root) return;
    binary_tree_node_in_order_0(root->left, cb, arg);
    cb(root, arg);
    binary_tree_node_in_order_0(root->right, cb, arg);
}

static void binary_tree_node_in_order_1(binary_tree_node_t *root, void(*cb)(binary_tree_node_t *, void *), void *arg)
{
    if (!root) return;
    algo_stack_t *stack = stack_alloc();
    binary_tree_node_t *node = root;
    while (node || stack_size(stack) != 0) {
        if (!node) {
            node = (binary_tree_node_t *)stack_pop(stack);
            cb(node, arg);
            node = node->right;
            continue;
        }
        stack_push(stack, node);
        node = node->left;
    }
    stack_free(stack, NULL);
}

static void binary_tree_node_in_order_2(binary_tree_node_t *root, void(*cb)(binary_tree_node_t *, void *), void *arg)
{
    if (!root) return;
    binary_tree_node_t *pre, *node;
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

static void binary_tree_node_post_order_0(binary_tree_node_t *root, void(*cb)(binary_tree_node_t *, void *), void *arg)
{
    if (!root) return;
    binary_tree_node_post_order_0(root->left, cb, arg);
    binary_tree_node_post_order_0(root->right, cb, arg);
    cb(root, arg);
}

static void binary_tree_node_post_order_1(binary_tree_node_t *root, void(*cb)(binary_tree_node_t *, void *), void *arg)
{
    if (!root) return;
    binary_tree_node_t *pre, *node;
    algo_stack_t *stack = stack_alloc();
    algo_stack_t *stack2 = stack_alloc();
    stack_push(stack, root);
    while (stack_size(stack) != 0) {
        node = (binary_tree_node_t *)stack_pop(stack);
        stack_push(stack2, node);
        if (node->left) stack_push(stack, node->left);
        if (node->right) stack_push(stack, node->right);
    }
    while (stack_size(stack2) != 0) {
        node = (binary_tree_node_t *)stack_pop(stack2);
        cb(node, arg);
    }
}

static void binary_tree_node_reverse(binary_tree_node_t *n1, binary_tree_node_t *n2)
{
    if (n1 == n2) return;
    binary_tree_node_t *x = n1;
    binary_tree_node_t *y = n1->right;
    do {
        binary_tree_node_t *tmp = y->right;
        y->right = x;
        x = y;
        y = tmp;
    } while (x != n2);
}

static void binary_tree_node_reverse_cb(binary_tree_node_t *n1, binary_tree_node_t *n2, void(*cb)(binary_tree_node_t *, void *), void *arg)
{
    binary_tree_node_reverse(n1, n2);
    binary_tree_node_t *node = n2;
    while (node != n1) {
        cb(node, arg);
        node = node->right;
    }
    cb(node, arg);
    binary_tree_node_reverse(n2, n1);
}

static void binary_tree_node_post_order_2(binary_tree_node_t *root, void(*cb)(binary_tree_node_t *, void *), void *arg)
{
    if (!root) return;
    binary_tree_node_t *pre, *node, *dummy;
    dummy = (binary_tree_node_t *)malloc(sizeof(binary_tree_node_t));
    dummy->parent = NULL;
    dummy->right = NULL;
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
        binary_tree_node_reverse_cb(node->left, pre, cb, arg);
        pre->right = NULL;
        node = node->right;
    }
    free(dummy);
}

inline static void binary_tree_node_in_order(binary_tree_node_t *root, void(*cb)(binary_tree_node_t *, void *), void *arg)
{
    binary_tree_node_in_order_2(root, cb, arg);
}

inline static void binary_tree_node_pre_order(binary_tree_node_t *root, void(*cb)(binary_tree_node_t *, void *), void *arg)
{
    binary_tree_node_pre_order_2(root, cb, arg);
}

inline static void binary_tree_node_post_order(binary_tree_node_t *root, void(*cb)(binary_tree_node_t *, void *), void *arg)
{
    binary_tree_node_post_order_2(root, cb, arg);
}

inline static binary_tree_node_t *binary_tree_node_alloc(void *data)
{
    binary_tree_node_t *node;
    node = (binary_tree_node_t *)malloc(sizeof(binary_tree_node_t));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

inline static void binary_tree_node_free(binary_tree_node_t *node, void *arg)
{
    if (!node) return;
    if (arg && node->data) {
        void(*dfree)(void *);
        dfree = (void(*)(void *))arg;
        dfree(node->data);
    }
    free(node);
}

inline static void binary_tree_node_clear(binary_tree_node_t *root, void(*dfree)(void *))
{
    binary_tree_node_post_order_0(root, binary_tree_node_free, dfree);
}

inline static int binary_tree_node_size(binary_tree_node_t *root)
{
    int size = 0;
    binary_tree_node_t *node;
    node = binary_tree_node_min(root);
    for ( ; node; node = binary_tree_node_post(node), size++);
    return size;
}

inline static binary_tree_node_t *binary_tree_node_max(binary_tree_node_t *root)
{
    binary_tree_node_t *node;
    if (!root) return NULL;
    for (node = root; node && node->right; node = node->right);
    return node;
}

inline static binary_tree_node_t *binary_tree_node_min(binary_tree_node_t *root)
{
    binary_tree_node_t *node;
    if (!root) return NULL;
    for (node = root; node && node->left; node = node->left);
    return node;
}

inline static binary_tree_node_t *binary_tree_node_pre(binary_tree_node_t *root)
{
    binary_tree_node_t *node;
    if (!root) return NULL;
    if (root->left) return binary_tree_node_max(root->left);
    for (node = root->parent; node && node->left == root; root = node, node = node->parent);
    return node;
}

inline static binary_tree_node_t *binary_tree_node_post(binary_tree_node_t *root)
{
    binary_tree_node_t *node;
    if (!root) return NULL;
    if (root->right) return binary_tree_node_min(root->right);
    for (node = root->parent; node && node->right == root; root = node, node = node->parent);
    return node;
}

inline static binary_tree_node_t *binary_tree_node_at(binary_tree_node_t *root, int n)
{
    binary_tree_node_t *node;
    node = binary_tree_node_min(root);
    for ( ; node && n > 0; node = binary_tree_node_post(node), n--);
    return node;
}

inline static binary_tree_node_t *binary_tree_node_find(binary_tree_node_t *root, void *data, int(*cmp)(void *, void *))
{
    binary_tree_node_t *node;
    for (node = root; node; ) {
        int ret = cmp(data, node->data);
        if (ret == 0) return node;
        node = ret > 0 ? node->right : node->left;
    }
    return NULL;
}

inline static void binary_tree_node_push(binary_tree_node_t **root, binary_tree_node_t *node, int(*cmp)(void *, void *))
{
    binary_tree_node_t *temp, *parent = NULL;
    for (temp = *root; temp; ) {
        parent = temp;
        temp = cmp(node->data, temp->data) > 0 ? temp->right : temp->left;
    }
    node->parent = parent;
    if (!parent) {
        *root = node;
    } else if (cmp(node->data, parent->data) > 0) {
        parent->right = node;
    } else {
        parent->left = node;
    }
}

inline static binary_tree_node_t *binary_tree_node_pop_leaf(binary_tree_node_t *node)
{
    if (node->parent->left == node) {
        node->parent->left = NULL;
    } else {
        node->parent->right = NULL;
    }
    return NULL;
}

inline static binary_tree_node_t *binary_tree_node_pop_who_with_one_child(binary_tree_node_t *node)
{
    binary_tree_node_t *child;
    child = node->left ? node->left : node->right;
    if (node->parent->left == node) {
        node->parent->left = child;
    } else {
        node->parent->right = child;
    }
    child->parent = node->parent;
    return child;
}

inline static binary_tree_node_t *binary_tree_node_pop_who_with_children(binary_tree_node_t *node)
{
    binary_tree_node_t *temp;
    // remove replaced node first
    temp = binary_tree_node_pre(node);
    if (!temp->left && !temp->right) {
        binary_tree_node_pop_leaf(temp);
    } else {
        binary_tree_node_pop_who_with_one_child(temp);
    }
    // swap replaced node and node
    if (!node->parent) {
    } else if (node->parent->left == node) {
        node->parent->left = temp;
    } else {
        node->parent->right = temp;
    }
    node->left->parent = temp;
    node->right->parent = temp;
    temp->parent = node->parent;
    temp->right = node->right;
    temp->left = node->left;
    return temp;
}

/**
 * @return replaced node. NULL means leaf node
 */
inline static binary_tree_node_t *binary_tree_node_pop(binary_tree_node_t **root, binary_tree_node_t *node)
{
    binary_tree_node_t *temp = NULL;
    if (!node) {
        return NULL;
    } else if (!node->left && !node->right) {
        temp = binary_tree_node_pop_leaf(node);
    } else if (node->left && node->right) {
        temp = binary_tree_node_pop_who_with_children(node);
    } else {
        temp = binary_tree_node_pop_who_with_one_child(node);
    }
    if (temp && !temp->parent) {
        *root = temp;
    }
    return temp;
}

inline static binary_tree_node_t *binary_tree_node_pop_front(binary_tree_node_t **root)
{
    binary_tree_node_t *node;
    node = binary_tree_node_min(*root);
    return binary_tree_node_pop(root, node);
}

inline static binary_tree_node_t *binary_tree_node_pop_back(binary_tree_node_t **root)
{
    binary_tree_node_t *node;
    node = binary_tree_node_max(*root);
    return binary_tree_node_pop(root, node);
}

#if defined(ENABLE_BINARY_TREE_TEST)
int binary_tree_node_int_less_than(void *data1, void *data2)
{
    return *(int *)data1 - *(int *)data2;
}
void binary_tree_node_print(binary_tree_node_t *node, void *arg)
{
    printf("%d ", *(int *)node->data);
}
int main(int argc, char **argv)
{
    int int_array[] = {6,4,3,2,7,5,9,0,1,8};
    binary_tree_node_t *root = binary_tree_node_alloc(&int_array[0]);
    binary_tree_node_t *node1 = binary_tree_node_alloc(&int_array[1]);
    binary_tree_node_t *node2 = binary_tree_node_alloc(&int_array[2]);
    binary_tree_node_t *node3 = binary_tree_node_alloc(&int_array[3]);
    binary_tree_node_t *node4 = binary_tree_node_alloc(&int_array[4]);
    binary_tree_node_t *node5 = binary_tree_node_alloc(&int_array[5]);
    binary_tree_node_t *node6 = binary_tree_node_alloc(&int_array[6]);
    binary_tree_node_t *node7 = binary_tree_node_alloc(&int_array[7]);
    binary_tree_node_t *node8 = binary_tree_node_alloc(&int_array[8]);
    binary_tree_node_t *node9 = binary_tree_node_alloc(&int_array[9]);
    binary_tree_node_push(&root, node1, binary_tree_node_int_less_than);
    binary_tree_node_push(&root, node2, binary_tree_node_int_less_than);
    binary_tree_node_push(&root, node3, binary_tree_node_int_less_than);
    binary_tree_node_push(&root, node4, binary_tree_node_int_less_than);
    binary_tree_node_push(&root, node5, binary_tree_node_int_less_than);
    binary_tree_node_push(&root, node6, binary_tree_node_int_less_than);
    binary_tree_node_push(&root, node7, binary_tree_node_int_less_than);
    binary_tree_node_push(&root, node8, binary_tree_node_int_less_than);
    binary_tree_node_push(&root, node9, binary_tree_node_int_less_than);
    assert(*(int *)binary_tree_node_at(root, 0)->data == 0);
    assert(*(int *)binary_tree_node_at(root, 1)->data == 1);
    assert(*(int *)binary_tree_node_at(root, 8)->data == 8);
    assert(*(int *)binary_tree_node_min(root)->data == 0);
    assert(*(int *)binary_tree_node_max(root)->data == 9);
    assert(binary_tree_node_size(root) == 10);
    binary_tree_node_in_order(root, binary_tree_node_print, NULL);
    printf("\n");
    binary_tree_node_pre_order(root, binary_tree_node_print, NULL);
    printf("\n");
    binary_tree_node_post_order(root, binary_tree_node_print, NULL);
    printf("\n");
    binary_tree_node_t *node;
    node = binary_tree_node_find(root, &int_array[7], binary_tree_node_int_less_than);
    binary_tree_node_pop(&root, node);
    binary_tree_node_in_order(root, binary_tree_node_print, NULL);
    printf("\n");
    binary_tree_node_pop_front(&root);
    binary_tree_node_pop_back(&root);
    binary_tree_node_in_order(root, binary_tree_node_print, NULL);
    printf("\n");

    binary_tree_node_clear(root, NULL);

    return 0;
}
#endif