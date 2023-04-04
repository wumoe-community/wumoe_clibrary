#ifndef WUMOE_QUEUE_H
#define WUMOE_QUEUE_H

#include <stddef.h>

typedef struct wqueue wqueue;

wqueue *new_queue();

size_t queue_size(wqueue *stack);

void *queue_peek(wqueue *stack);

void *queue_pop(wqueue *stack);

void queue_push(wqueue *stack, void *value);

void queue_free(wqueue *stack);

#endif
