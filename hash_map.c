#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "hash_map.h"

typedef struct hash_map_node {
    void *key;
    void *value;
    struct hash_map_node *next;
} hash_map_node_t;

typedef struct hash_map {
    hash_map_node_t *table;
    size_t size;
} hash_map_t;

