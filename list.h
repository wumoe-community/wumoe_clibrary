#ifndef WUMOE_LIST_H
#define WUMOE_LIST_H

#include <stddef.h>

typedef struct list list;

list *new_list();

void list_add(list *list, void *value);

void list_set(list *list, size_t index, void *value);

void list_insert(list *list, size_t index, void *value);

size_t list_size(list *list);

void *list_get(list *list, size_t index);

void *list_remove(list *list, size_t index);

void list_free(list *list);

#endif
