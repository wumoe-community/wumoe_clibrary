#ifndef WUMOE_STACK_H
#define WUMOE_STACK_H

typedef struct wstack wstack;

wstack *new_stack();

size_t stack_size(wstack *stack);

void *stack_peek(wstack *stack);

void *stack_pop(wstack *stack);

void stack_push(wstack *stack, void *value);

void stack_free(wstack *stack);

#endif
