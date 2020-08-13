#ifndef __MAP_H__
#define __MAP_H__

typedef struct rb_tree map_t;

map_t *map_alloc();
void map_free(map_t *map);
void map_clear(map_t *map);
int  map_size(map_t *map);

void map_push(map_t *map, void *key, void *value);
void map_pop(map_t *map, void *key);

void *map_find(map_t *map, void *key);
void *map_front(map_t *map);
void *map_back(map_t *map);

void *map_begin(map_t *map);
void *map_end(map_t *map);
void *map_next(map_t *map);
void  map_foreach(map_t *map, void(*cb)(void *, void *, void *), void *arg);

#endif