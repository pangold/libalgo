#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "rb_tree.h"
#include "map.h"

typedef struct kv_element {
    void *key;
    void *value;
} kv_element_t;

typedef struct map {
    rb_tree_t *rb_tree;
    void *(*allock)(void *);
    void *(*allocv)(void *);
    void  (*freek)(void *);
    void  (*freev)(void *);
} map_t;

/**
 * int
 */
static int __map_key_int_cmp(void *data1, void *data2)
{
    kv_element_t *ele1 = (kv_element_t *)data1;
    kv_element_t *ele2 = (kv_element_t *)data2;
    return *(int *)ele1->key - *(int *)ele2->key;
}

static void *__map_key_int_alloc(void *data)
{
    void *key = malloc(sizeof(int));
    memcpy(key, data, sizeof(int));
    return key;
}

/**
 * string
 */
static int __map_key_str_cmp(void *data1, void *data2)
{
    kv_element_t *ele1 = (kv_element_t *)data1;
    kv_element_t *ele2 = (kv_element_t *)data2;
    return strcmp((char *)ele1->key, (char *)ele2->key);
}

static void *__map_key_str_alloc(void *data)
{
    char *str = (char *)data;
    int len = strlen(str);
    char *key = (char *)malloc(len);
    memcpy(key, data, len);
    return key;
}

/**
 * API
 */
map_t *map_alloc_int()
{
    map_t *map = (map_t *)malloc(sizeof(map_t));
    map->rb_tree = rb_tree_alloc(__map_key_int_cmp);
    map->allock = __map_key_int_alloc;
    map->allocv = NULL;
    map->freek = free;
    map->freev = NULL;
    return map;
}

map_t *map_alloc_str()
{
    map_t *map = (map_t *)malloc(sizeof(map_t));
    map->rb_tree = rb_tree_alloc(__map_key_str_cmp);
    map->allock = __map_key_str_alloc;
    map->allocv = NULL;
    map->freek = free;
    map->freev = NULL;
    return map;
}

static void __map_free_cb(void *data, void *arg)
{
    map_t *map = (map_t *)arg;
    kv_element_t *kv = (kv_element_t *)data;
    if (map->freek) map->freek(kv->key);
    if (map->freev) map->freev(kv->value);
}

void map_clear(map_t *map)
{
    if (!map) return;
    if (map->freek || map->freev)
        rb_tree_foreach(map->rb_tree, __map_free_cb, map);
    rb_tree_clear(map->rb_tree, free);
}

void map_free(map_t *map)
{
    if (!map) return;
    map_clear(map);
    free(map);
}

size_t map_size(map_t *map)
{
    if (!map) return 0;
    return rb_tree_size(map->rb_tree);
}

void map_push(map_t *map, void *key, void *value)
{
    if (!map) return;
    kv_element_t *kv = (kv_element_t *)malloc(sizeof(kv_element_t));
    kv->key = map->allock ? map->allock(key) : key;
    kv->value = map->allocv ? map->allocv(value) : value;
    rb_tree_push(map->rb_tree, kv);
}

void *map_pop(map_t *map, void *key)
{
    kv_element_t kv, *ret;
    kv.key = key;
    ret = (kv_element_t *)rb_tree_pop(map->rb_tree, &kv);
    if (map->freek) map->freek(ret->key);
    free(ret);
    return ret->value;
}

void *map_find(map_t *map, void *key)
{
    kv_element_t kv, *ret;
    kv.key = key;
    ret = (kv_element_t *)rb_tree_find(map->rb_tree, &kv);
    return ret->value;
}

struct __map_foreach_user_cb {
    void (*func)(void *, void *, void *);
    void *arg;
};

static void __map_foreach_cb(void *data, void *arg)
{
    kv_element_t *kv = (kv_element_t *)data;
    struct __map_foreach_user_cb *ucb = (struct __map_foreach_user_cb *)arg;
    ucb->func(kv->key, kv->value, ucb->arg);
}

void map_foreach(map_t *map, void(*cb)(void *, void *, void *), void *arg)
{
    struct __map_foreach_user_cb ucb = { cb, arg };
    rb_tree_foreach(map->rb_tree, __map_foreach_cb, &ucb);
}

#if defined(ENABLE_MAP_TEST)
static void __map_print(void *key, void *value, void *arg)
{
    printf("key: %d, value: %d\n", *(int *)key, *(int *)value);
}
void map_test()
{
    int key[] = { 2,5,6,4,3,1,8,9,7,0 };
    int value[] = { 22,55,66,44,33,11,88,99,77,10 };
    map_t *map = map_alloc_int();
    map_push(map, &key[0], &value[0]);
    map_push(map, &key[1], &value[1]);
    map_push(map, &key[2], &value[2]);
    map_push(map, &key[3], &value[3]);
    map_push(map, &key[4], &value[4]);
    map_push(map, &key[5], &value[5]);
    map_push(map, &key[6], &value[6]);
    map_push(map, &key[7], &value[7]);
    map_push(map, &key[8], &value[8]);
    map_push(map, &key[9], &value[9]);
    assert(*(int *)map_find(map, &key[9]) == 10);
    map_foreach(map, __map_print, NULL);
    map_pop(map, &key[9]);
    assert(map_size(map) == 9);
    map_clear(map);
    assert(map_size(map) == 0);
    map_free(map);
}
#endif