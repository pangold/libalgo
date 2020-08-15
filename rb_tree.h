#ifndef __RB_TREE_H__
#define __RB_TREE_H__

typedef struct rb_tree rb_tree_t;
typedef void (*rb_tree_free_t)(void *);
typedef int (*rb_tree_compare_t)(void *, void *);

rb_tree_t *rb_tree_alloc(rb_tree_compare_t compare);
void rb_tree_clear(rb_tree_t *tree, rb_tree_free_t freecb);
void rb_tree_free(rb_tree_t *tree, rb_tree_free_t freecb);
size_t rb_tree_size(rb_tree_t *tree);

void rb_tree_push(rb_tree_t *tree, void *data);
void *rb_tree_pop(rb_tree_t *tree, void *data);
void *rb_tree_pop_front(rb_tree_t *tree);
void *rb_tree_pop_back(rb_tree_t *tree);

void *rb_tree_front(rb_tree_t *tree);
void *rb_tree_back(rb_tree_t *tree);
void *rb_tree_find(rb_tree_t *tree, void *data);

void rb_tree_foreach(rb_tree_t *tree, void(*cb)(void *, void *), void *arg);

#endif