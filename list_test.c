#include <stdio.h>

#include "list.h"

int main(int argc, char** argv)
{
    int n, i = 0;
    struct list *list = list_alloc(sizeof(int));
    struct list_node *node;

    n = 111;
    list_push_at(list, &n, 0);
    n = 222;
    list_push_back(list, &n);
    n = 333;
    list_push_front(list, &n);
    n = 444;
    list_push_at(list, &n, 2);
    n = 555;
    list_push_at(list, &n, 0);
    n = 666;
    list_push_at(list, &n, 5);

    for (i = 0, node = list->head; node; node = node->next, i++) {
        printf("(%d) %d %d\n", list->size, i, *(int*)node->data);
    }
    printf("\n");

    list_pop_at(list, 5);
    list_pop_at(list, 5);
    list_pop_at(list, 0);
    list_pop_back(list);
    list_pop_front(list);

    for (i = 0, node = list->head; node; node = node->next, i++) {
        printf("(%d) %d %d\n", list->size, i, *(int*)node->data);
    }
    printf("\n");

    for (i = 0, node = list->tail; node; node = node->pre, i++) {
        printf("(%d) %d %d\n", list->size, i, *(int*)node->data);
    }
    printf("\n");

    list_free(list);
    
    return 1;
}