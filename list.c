#include <stdlib.h>
#include "list.h"

#define LIST_DEFAULT_CAPACITY  10

typedef struct list {
    size_t size;
    size_t capacity;
    void **array;
} list;

void list_resize(list *list);

list *new_list() {
    list *new_list = (list *) malloc(sizeof(list));
    new_list->capacity = LIST_DEFAULT_CAPACITY;
    new_list->size = 0;
    new_list->array = (void **) malloc(sizeof(void *) * new_list->capacity);
    for (int i = 0; i < new_list->capacity; ++i)
        new_list->array[i] = NULL;
    return new_list;
}

void list_add(list *list, void *value) {
    if (list->size >= list->capacity)
        list_resize(list);
    list->array[list->size++] = value;
}

void list_set(list *list, size_t index, void *value) {
    list->array[index] = value;
}

void list_insert(list *list, size_t index, void *value) {
    if (list->size >= list->capacity)
        list_resize(list);
    for (int i = list->size; i >= index; --i)
        list->array[i + 1] = list->array[i];
    list->array[index] = value;
}

size_t list_size(list *list) {
    return list->size;
}

void *list_get(list *list, size_t index) {
    return list->array[index];
}

void *list_remove(list *list, size_t index) {
    void *result = list->array[index];
    list->array[index] = NULL;
    return result;
}

void list_free(list *list) {
    free(list);
}

void list_resize(list *list) {
    size_t new_capacity = list->capacity + (list->capacity >> 1);
    void **new_array = (void **) malloc(sizeof(void *) * new_capacity);
    for (int i = 0; i < list->size; ++i)
        new_array[i] = list->array[i];
    free(list->array);
    list->array = new_array;
}
