#ifndef __LIST_H__
#define __LIST_H__

struct list_node {
    void *data;
    struct list_node *pre;
    struct list_node *next;
};

struct list {
    struct list_node *head;
    struct list_node *tail;
    int size;
    int sizeof_type;
};

// create(C)
struct list *list_alloc(int sizeof_type);
void list_free(struct list *list);
void list_clear(struct list *list);

// insert(U)
int list_push_at(struct list *list, void *data, int index);
int list_push_back(struct list *list, void *data);
int list_push_front(struct list *list, void *data);

// remove(D)
int list_pop_at(struct list *list, int index);
int list_pop_back(struct list *list);  // 
int list_pop_front(struct list *list); 

// query(R)
void *list_begin(struct list *list);
void *list_end(struct list *list);
void *list_at(struct list *list, int index);
void  list_foreach(struct list *list, int(*cb)(void *, void *), void *arg);

int list_empty(struct list *list);
int list_size(struct list *list);


#endif