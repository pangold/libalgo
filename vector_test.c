#include <stdio.h>

#include "vector.h"

int main(int argc, char** argv)
{
    int i, n, size, *v;
    struct vector *vec = vector_alloc(sizeof(int), 0);
    
    n = 111;
    vector_push_back(vec, &n);
    n = 222;
    vector_push_back(vec, &n);
    n = 333;
    vector_push_at(vec, &n, 0);
    n = 444;
    vector_push_at(vec, &n, 1);
    n = 555;
    vector_push_at(vec, &n, 2);
    n = 666;
    vector_push_at(vec, &n, 3);

    v = vector_begin(vec);
    size = vector_size(vec);
    for (i = 0; i < size; i++) {
        printf("index %d, value: %d\n", i, v[i]);
    }
    printf("\n");

    vector_pop_at(vec, 0);
    vector_pop_back(vec);
    vector_pop_front(vec);

    v = vector_begin(vec);
    size = vector_size(vec);
    for (i = 0; i < size; i++) {
        printf("index %d, value: %d\n", i, v[i]);
    }
    printf("\n");
    
    vector_free(vec);
    
    return 1;
}