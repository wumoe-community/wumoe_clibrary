#include <stdlib.h>
#include "wstack.h"

typedef struct stack_node {
    void *value;
    struct stack_node *next;
} stack_node;

typedef struct wstack {
    stack_node *head;
    size_t size;
} stack;

wstack *new_stack() {
    wstack *new_stack = (wstack *) malloc(sizeof(wstack));
    new_stack->head = NULL;
    new_stack->size = 0;
}

size_t stack_size(wstack *stack) {
    return stack->size;
}

void *stack_peek(wstack *stack) {
    if (stack->head == NULL)
        return NULL;
    return stack->head->value;
}

void *stack_pop(wstack *stack) {
    void *result = stack_peek(stack);
    if (result == NULL)
        return NULL;
    stack_node *next = stack->head->next;
    free(stack->head);
    stack->head = next;
    --stack->size;
    return result;
}

void stack_push(wstack *stack, void *value) {
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

void stack_free(wstack *stack) {
    while (stack->size > 0)
        stack_pop(stack);
    free(stack);
}
