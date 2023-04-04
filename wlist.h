#ifndef WUMOE_LIST_H
#define WUMOE_LIST_H

#include <stddef.h>

typedef struct wlist wlist;

wlist *new_list();

void list_add(wlist *list, void *value);

void list_set(wlist *list, size_t index, void *value);

void list_insert(wlist *list, size_t index, void *value);

size_t list_size(wlist *list);

void *list_get(wlist *list, size_t index);

void *list_remove(wlist *list, size_t index);

void list_free(wlist *list);

#endif
