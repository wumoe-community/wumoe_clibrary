#ifndef WUMOE_QUEUE_H
#define WUMOE_QUEUE_H

#include <stddef.h>

typedef struct queue queue;

queue *new_queue();

size_t queue_size(queue *stack);

void *queue_peek(queue *stack);

void *queue_pop(queue *stack);

void queue_push(queue *stack, void *value);

void queue_free(queue *stack);

#endif
