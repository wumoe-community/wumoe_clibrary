#ifndef WUMOE_TABLE_H
#define WUMOE_TABLE_H

typedef struct wtable wtable;

wtable *new_table();

size_t table_size(wtable *table);

int table_contains(wtable *table, char *key);

void table_push(wtable *table, char *key, void *value);

void *table_get(wtable *table, char *key);

void *table_remove(wtable *table, char *key);

char **table_keys(wtable *table);

void **table_values(wtable *table);

void table_free(wtable *table);

#endif
