#include <stdlib.h>
#include "queue.h"

typedef struct queue_node {
    void *value;
    struct queue_node *next;
} queue_node;

typedef struct queue {
    queue_node *head;
    size_t size;
} queue;

queue *new_queue() {
    queue *new_queue = (queue *) malloc(sizeof(queue));
    new_queue->head = NULL;
    new_queue->size = 0;
}

size_t queue_size(queue *queue) {
    return queue->size;
}

void *queue_peek(queue *queue) {
    if (queue->head == NULL)
        return NULL;
    return queue->head->value;
}

void *queue_pop(queue *queue) {
    void *result = queue_peek(queue);
    if (result == NULL)
        return NULL;
    queue_node *next = queue->head->next;
    free(queue->head);
    queue->head = next;
    --queue->size;
    return result;
}

void queue_push(queue *queue, void *value) {
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

void queue_free(queue *queue) {
    while (queue->size > 0)
        queue_pop(queue);
    free(queue);
}
