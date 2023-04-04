#include <stdlib.h>
#include "wlist.h"

#define LIST_DEFAULT_CAPACITY  10

typedef struct wlist {
    size_t size;
    size_t capacity;
    void **array;
} list;

void list_resize(wlist *list);

wlist *new_list() {
    wlist *new_list = (wlist *) malloc(sizeof(wlist));
    new_list->capacity = LIST_DEFAULT_CAPACITY;
    new_list->size = 0;
    new_list->array = (void **) malloc(sizeof(void *) * new_list->capacity);
    for (int i = 0; i < new_list->capacity; ++i)
        new_list->array[i] = NULL;
    return new_list;
}

void list_add(wlist *list, void *value) {
    if (list->size >= list->capacity)
        list_resize(list);
    list->array[list->size++] = value;
}

void list_set(wlist *list, size_t index, void *value) {
    list->array[index] = value;
}

void list_insert(wlist *list, size_t index, void *value) {
    if (list->size >= list->capacity)
        list_resize(list);
    for (int i = list->size; i >= index; --i)
        list->array[i + 1] = list->array[i];
    list->array[index] = value;
}

size_t list_size(wlist *list) {
    return list->size;
}

void *list_get(wlist *list, size_t index) {
    return list->array[index];
}

void *list_remove(wlist *list, size_t index) {
    void *result = list->array[index];
    list->array[index] = NULL;
    return result;
}

void list_free(wlist *list) {
    free(list);
}

void list_resize(wlist *list) {
    size_t new_capacity = list->capacity + (list->capacity >> 1);
    void **new_array = (void **) malloc(sizeof(void *) * new_capacity);
    for (int i = 0; i < list->size; ++i)
        new_array[i] = list->array[i];
    free(list->array);
    list->array = new_array;
}
