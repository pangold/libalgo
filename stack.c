#include "stack.h"
#include "list.h"

algo_stack_t *stack_alloc()
{
    return list_alloc();
}

void stack_free(algo_stack_t *stack, void(*freecb)(void *))
{
    list_free(stack, freecb);
}

void stack_clear(algo_stack_t *stack, void(*freecb)(void *))
{
    list_clear(stack, freecb);
}

int stack_size(algo_stack_t *stack)
{
    return list_size(stack);
}

void stack_push(algo_stack_t *stack, void *data)
{
    list_push_front(stack, data);
}

void *stack_pop(algo_stack_t *stack)
{
    return list_pop_front(stack);
}

void *algo_stack_top(algo_stack_t *stack)
{
    return list_front(stack);
}