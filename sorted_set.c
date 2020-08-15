#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "rb_tree.h"
#include "sorted_set.h"


sorted_set_t *sorted_set_alloc(sorted_set_compare_t compare)
{
    return rb_tree_alloc(compare);
}

void sorted_set_free(sorted_set_t *set, sorted_set_free_t cb)
{
    rb_tree_free(set, cb);
}

void sorted_set_clear(sorted_set_t *set, sorted_set_free_t cb)
{
    rb_tree_clear(set, cb);
}

size_t sorted_set_size(sorted_set_t *set)
{
    return rb_tree_size(set);
}

void sorted_set_push(sorted_set_t *set, void *data)
{
    rb_tree_push(set, data);
}

void *sorted_set_pop(sorted_set_t *set, void *data)
{
    return rb_tree_pop(set, data);
}

void *sorted_set_pop_front(sorted_set_t *set)
{
    return rb_tree_pop_front(set);
}

void *sorted_set_pop_back(sorted_set_t *set)
{
    return rb_tree_pop_back(set);
}

void *sorted_set_front(sorted_set_t *set)
{
    return rb_tree_front(set);
}

void *sorted_set_back(sorted_set_t *set)
{
    return rb_tree_back(set);
}

void sorted_set_foreach(sorted_set_t *set, void(*cb)(void *, void *), void *arg)
{
    rb_tree_foreach(set, cb, arg);
}

#if defined(ENABLE_SORTED_SET_TEST)
static int __sorted_set_int_cmp(void *data1, void *data2)
{
    return *(int *)data1 - *(int *)data2;
}
static void __sorted_set_print(void *data, void *arg)
{
    printf("%d, ", *(int *)data);
}
void sorted_set_test() 
{
    sorted_set_t *set = sorted_set_alloc(__sorted_set_int_cmp);
    int key[] = { 2,5,6,4,3,1,8,9,7,0 };
    sorted_set_push(set, &key[0]);
    sorted_set_push(set, &key[1]);
    sorted_set_push(set, &key[2]);
    sorted_set_push(set, &key[3]);
    sorted_set_push(set, &key[4]);
    sorted_set_push(set, &key[5]);
    sorted_set_push(set, &key[6]);
    sorted_set_push(set, &key[7]);
    sorted_set_push(set, &key[8]);
    sorted_set_push(set, &key[9]);
    sorted_set_foreach(set, __sorted_set_print, NULL);
    printf("\n");
    assert(sorted_set_size(set) == 10);
    sorted_set_pop(set, &key[9]);
    assert(*(int *)sorted_set_front(set) == 1);
    assert(sorted_set_size(set) == 9);
    assert(*(int *)sorted_set_pop_back(set) == 9);
    assert(*(int *)sorted_set_back(set) == 8);
    assert(sorted_set_size(set) == 8);
    sorted_set_clear(set, NULL);
    assert(sorted_set_size(set) == 0);
    sorted_set_free(set, NULL);
}
#endif