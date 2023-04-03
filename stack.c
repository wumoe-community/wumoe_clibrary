#include <stdlib.h>
#include "stack.h"

typedef struct stack_node {
    void *value;
    struct stack_node *next;
} stack_node;

typedef struct stack {
    stack_node *head;
    size_t size;
} stack;

stack *new_stack() {
    stack *new_stack = (stack *) malloc(sizeof(stack));
    new_stack->head = NULL;
    new_stack->size = 0;
}

size_t stack_size(stack *stack) {
    return stack->size;
}

void *stack_peek(stack *stack) {
    if (stack->head == NULL)
        return NULL;
    return stack->head->value;
}

void *stack_pop(stack *stack) {
    void *result = stack_peek(stack);
    if (result == NULL)
        return NULL;
    stack_node *next = stack->head->next;
    free(stack->head);
    stack->head = next;
    --stack->size;
    return result;
}

void stack_push(stack *stack, void *value) {
    stack_node *new_node = (stack_node *) malloc(sizeof(stack_node));
    new_node->value = value;
    new_node->next = NULL;
    ++stack->size;
    if (stack->head == NULL) {
        stack->head = new_node;
        return;
    }
    new_node->next = stack->head;
    stack->head = new_node;
}

void stack_free(stack *stack) {
    while (stack->size > 0)
        stack_pop(stack);
    free(stack);
}
