#ifndef __MAP_H__
#define __MAP_H__

typedef struct map map_t;

map_t *map_alloc_int();
map_t *map_alloc_str();
void map_free(map_t *map);
void map_clear(map_t *map);
size_t map_size(map_t *map);

void map_push(map_t *map, void *key, void *value);
void *map_pop(map_t *map, void *key);
void *map_find(map_t *map, void *key);

void map_foreach(map_t *map, void(*cb)(void *, void *, void *), void *arg);

#endif