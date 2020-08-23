#ifndef __STACK_H__
#define __STACK_H__

typedef struct list algo_stack_t;

algo_stack_t *stack_alloc();
void stack_free(algo_stack_t *stack, void(*freecb)(void *));
void stack_clear(algo_stack_t *stack, void(*freecb)(void *));
int  stack_size(algo_stack_t *stack);

void  stack_push(algo_stack_t *stack, void *data);
void *stack_pop(algo_stack_t *stack);
void *algo_stack_top(algo_stack_t *stack);

#endif