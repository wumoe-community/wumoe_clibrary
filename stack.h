#ifndef WUMOE_STACK_H
#define WUMOE_STACK_H

typedef struct stack stack;

stack *new_stack();

size_t stack_size(stack *stack);

void *stack_peek(stack *stack);

void *stack_pop(stack *stack);

void stack_push(stack *stack, void *value);

void stack_free(stack *stack);

#endif
