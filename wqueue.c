#include <stdlib.h>
#include "wqueue.h"

typedef struct queue_node {
    void *value;
    struct queue_node *next;
} queue_node;

typedef struct wqueue {
    queue_node *head;
    size_t size;
} queue;

wqueue *new_queue() {
    wqueue *new_queue = (wqueue *) malloc(sizeof(wqueue));
    new_queue->head = NULL;
    new_queue->size = 0;
}

size_t queue_size(wqueue *queue) {
    return queue->size;
}

void *queue_peek(wqueue *queue) {
    if (queue->head == NULL)
        return NULL;
    return queue->head->value;
}

void *queue_pop(wqueue *queue) {
    void *result = queue_peek(queue);
    if (result == NULL)
        return NULL;
    queue_node *next = queue->head->next;
    free(queue->head);
    queue->head = next;
    --queue->size;
    return result;
}

void queue_push(wqueue *queue, void *value) {
    queue_node *new_node = (queue_node *) malloc(sizeof(queue_node));
    new_node->value = value;
    new_node->next = NULL;
    ++queue->size;
    queue_node * temp = queue->head;
    if(!temp)
        queue->head = new_node;
    else {
        while(temp->next)
            temp = temp->next;
        temp->next = new_node;
    }
}

void queue_free(wqueue *queue) {
    while (queue->size > 0)
        queue_pop(queue);
    free(queue);
}
