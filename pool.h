#ifndef __POOL_H__
#define __POOL_H__

struct pool {
    char *content;
    int   capacity;
    int   pos;
    struct allocator *next;
};

int pool_init(struct pool *dst, int capacity);
int pool_exit(struct pool *dst);

void *pool_alloc(struct pool *dst, void *data, int size);
void *pool_free(struct pool *dst, void *data, int size);

#endif