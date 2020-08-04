#include "stack.h"
#include "list.h"

stack_t *stack_alloc()
{
    return list_alloc();
}

void stack_free(stack_t *stack, void(*freecb)(void *))
{
    list_free(stack, freecb);
}

void stack_clear(stack_t *stack, void(*freecb)(void *))
{
    list_clear(stack, freecb);
}

int stack_size(stack_t *stack)
{
    return list_size(stack);
}

void stack_push(stack_t *stack, void *data)
{
    list_push_front(stack, data);
}

void *stack_pop(stack_t *stack)
{
    return list_pop_front(stack);
}

void *stack_top(stack_t *stack)
{
    return list_front(stack);
}