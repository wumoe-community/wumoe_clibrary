#ifndef WUMOE_TABLE_H
#define WUMOE_TABLE_H

typedef struct table table;

table *new_table();

size_t table_size(table *table);

int table_contains(table *table, char *key);

void table_push(table *table, char *key, void *value);

void *table_get(table *table, char *key);

void *table_remove(table *table, char *key);

char **table_keys(table *table);

void **table_values(table *table);

void table_free(table *table);

#endif
