
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(ENABLE_LIST_TEST)
#include "list.c"
#endif

#if defined(ENABLE_ARRAY_TEST)
#include "array.c"
#endif

#if defined(ENABLE_HEAP_TEST)
#if !defined(ENABLE_ARRAY_TEST)
#include "array.c"
#define ENABLE_ARRAY_TEST
#endif
#include "heap.c"
#endif

#if defined(ENABLE_RBTREE_TEST)
#if !defined(ENABLE_STACK_TEST)
#if !defined(ENABLE_LIST_TEST)
#include "list.c"
#define ENABLE_LIST_TEST
#endif
#include "stack.c"
#define ENABLE_STACK_TEST
#endif
#include "rb_tree.c"
#endif

#if defined(ENABLE_SORTED_SET_TEST)
#if !defined(ENABLE_RBTREE_TEST)
#if !defined(ENABLE_STACK_TEST)
#if !defined(ENABLE_LIST_TEST)
#include "list.c"
#define ENABLE_LIST_TEST
#endif
#include "stack.c"
#define ENABLE_STACK_TEST
#endif
#include "rb_tree.c"
#define ENABLE_RBTREE_TEST
#endif
#include "sorted_set.c"
#endif

#if defined(ENABLE_MAP_TEST)
#if !defined(ENABLE_RBTREE_TEST)
#if !defined(ENABLE_STACK_TEST)
#if !defined(ENABLE_LIST_TEST)
#include "list.c"
#define ENABLE_LIST_TEST
#endif
#include "stack.c"
#define ENABLE_STACK_TEST
#endif
#include "rb_tree.c"
#define ENABLE_RBTREE_TEST
#endif
#include "map.c"
#endif

int main(int argc, char **argv)
{

#if defined(ENABLE_LIST_TEST)
    list_int_test();
#endif

#if defined(ENABLE_ARRAY_TEST)
    array_int_test();
#endif

#if defined(ENABLE_HEAP_TEST)
    heap_int_test();
#endif

#if defined(ENABLE_RBTREE_TEST)
    rb_tree_test();
#endif

#if defined(ENABLE_SORTED_SET_TEST)
    sorted_set_test();
#endif

#if defined(ENABLE_MAP_TEST)
    map_test();
#endif

    return 0;
}

