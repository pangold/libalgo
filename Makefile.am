
## libalgo
##
AM_CFLAGS = -Wall -fno-strict-aliasing -std=gnu99 -D_GNU_SOURCE

lib_LTLIBRARIES = libalgo.la

libalgo_la_SOURCES = array.c
libalgo_la_SOURCES += heap.c
libalgo_la_SOURCES += list.c
libalgo_la_SOURCES += stack.c
libalgo_la_SOURCES += queue.c
libalgo_la_SOURCES += rb_tree.c
libalgo_la_SOURCES += sorted_set.c
libalgo_la_SOURCES += hash_map.c

libalgo_la_LDFLAGS = -static

libalgo_la_CFLAGS = $(AM_CFLAGS)

## algo_test
## 
bin_PROGRAMS = algo_test

alog_test_SOURCES = algo_test.c

algo_test_CFLAGS = $(AM_CFLAGS) -DENABLE_LIST_TEST -DENABLE_ARRAY_TEST -DENABLE_HEAP_TEST -DENABLE_RBTREE_TEST -DENABLE_SORTED_SET_TEST -DENABLE_MAP_TEST

algo_test_LDFLAGS = -static

algo_test_LDADD = $(SS_COMMON_LIBS)