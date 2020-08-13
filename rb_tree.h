#ifndef __RB_TREE_H__
#define __RB_TREE_H__

typedef struct rb_tree rb_tree_t;

rb_tree_t *rb_tree_alloc();
void rb_tree_clear(rb_tree_t *tree);
void rb_tree_free(rb_tree_t *tree);

void rb_tree_set_key_alloc(rb_tree_t *tree, void*(*alloc)(void *));
void rb_tree_set_value_alloc(rb_tree_t *tree, void*(*alloc)(void *));
void rb_tree_set_key_free(rb_tree_t *tree, void(*free)(void *));
void rb_tree_set_value_free(rb_tree_t *tree, void(*free)(void *));
void rb_tree_set_compare(rb_tree_t *tree, int(*compare)(void *, void *));

void rb_tree_push(rb_tree_t *tree, void *key, void *value);
void rb_tree_pop(rb_tree_t *tree, void *key);

void *rb_tree_find(rb_tree_t *tree, void *key);
void *rb_tree_at(rb_tree_t *tree, size_t n);
void *rb_tree_front(rb_tree_t *tree);
void *rb_tree_back(rb_tree_t *tree);

void *rb_tree_begin(rb_tree_t *tree);
void *rb_tree_end(rb_tree_t *tree);
void *rb_tree_next(rb_tree_t *tree);
void  rb_tree_foreach(rb_tree_t *tree, void(*cb)(void *, void *, void *), void *arg);

#endif