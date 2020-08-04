#ifndef __STACK_H__
#define __STACK_H__

typedef struct list stack_t;

stack_t *stack_alloc();
void stack_free(stack_t *stack, void(*freecb)(void *));
void stack_clear(stack_t *stack, void(*freecb)(void *));
int  stack_size(stack_t *stack);

void  stack_push(stack_t *stack, void *data);
void *stack_pop(stack_t *stack);
void *stack_top(stack_t *stack);

#endif