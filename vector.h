#ifndef __VECTOR_H__
#define __VECTOR_H__

struct vector;

// create(C)
struct vector *vector_alloc(int sizeof_type, int size);
void vector_free(struct vector *vec);

// insert(U)
int vector_push_at(struct vector *vec, void *data, int index);
int vector_push_back(struct vector *vec, void *data); // see sizeof_type, 
int vector_push_front(struct vector *vec, void *data);

// remove(D)
int vector_pop_at(struct vector *vec, int index);
int vector_pop_back(struct vector *vec);  // 
int vector_pop_front(struct vector *vec);

// query(R)
void *vector_begin(struct vector *vec);
void *vector_end(struct vector *vec);
void *vector_at(struct vector *vec, int index);
void  vector_foreach(struct vector *vec, int(*cb)(void *, void *), void *arg);

int vector_clear(struct vector *vec);
int vector_empty(struct vector *vec);
int vector_size(struct vector *vec);

// algorithm: find, sort, revert

#endif