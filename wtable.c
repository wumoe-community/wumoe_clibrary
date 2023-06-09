#include <stdlib.h>
#include <string.h>
#include "wtable.h"

#define TABLE_DEFAULT_CAPACITY  (1 << 4)
#define TABLE_DEFAULT_LODER     0.75f
#define TABLE_MAX_CAPACITY      (1 << 30)

#define HEIGHT(node)        (node == NULL ? 0 : node->height)
#define MAX(a, b)           (a > b ? a : b)
#define GET_BALANCE(node)   (node == NULL ? 0 : HEIGHT(node->left) - HEIGHT(node->right))

typedef struct table_entry {
    unsigned int hash;
    char *key;
    void *value;
    struct table_entry *next;
} table_entry;

typedef struct avl_tree_node {
    unsigned int key;
    int height;
    table_entry *entry;
    struct avl_tree_node *left;
    struct avl_tree_node *right;
    size_t avl_keys_index;
    size_t avl_values_index;
} avl_tree_node;

typedef struct wtable {
    avl_tree_node **avl_array;
    size_t capacity;
    size_t size;
} wtable;

void table_push_entry(wtable *table, table_entry *entry);

void table_resize(wtable *table);

unsigned int table_hash(char *key);

avl_tree_node *new_avl_node(unsigned int key, table_entry *entry);

avl_tree_node *avl_push(avl_tree_node *node, unsigned int key, table_entry *entry);

avl_tree_node *avl_remove(avl_tree_node *base, unsigned int key);

table_entry *avl_get_node(avl_tree_node *node, unsigned int key);

void avl_free(avl_tree_node *node);

wtable *new_table() {
    wtable *new_table = (wtable *) malloc(sizeof(wtable));
    new_table->capacity = TABLE_DEFAULT_CAPACITY;
    new_table->size = 0;
    new_table->avl_array = (avl_tree_node **) malloc(sizeof(avl_tree_node *) * new_table->capacity);
    for (size_t i = 0; i < new_table->capacity; ++i)
        new_table->avl_array[i] = NULL;
    return new_table;
}

size_t table_size(wtable *table) {
    return table->size;
}

int table_contains(wtable *table, char *key) {
    unsigned int key_hash = table_hash(key) & (table->capacity - 1);
    if (table->avl_array[key_hash] == NULL)
        return 0;
    avl_tree_node *avl = table->avl_array[key_hash];
    table_entry *entry = avl_get_node(avl, table_hash(key));
    while (entry != NULL && strcmp(key, entry->key) != 0)
        entry = entry->next;
    if (entry == NULL)
        return 0;
    return 1;
}

void table_push(wtable *table, char *key, void *value) {
    ++table->size;
    if (table->size >= (size_t) ((float) table->capacity * TABLE_DEFAULT_LODER))
        table_resize(table);
    table_entry *new_entry = (table_entry *) malloc(sizeof(table_entry));
    new_entry->hash = table_hash(key);
    char *new_key = (char *) malloc(sizeof(char) * strlen(key));
    strcpy(new_key, key);
    new_entry->key = new_key;
    new_entry->value = value;
    new_entry->next = NULL;
    table_push_entry(table, new_entry);
}

void *table_get(wtable *table, char *key) {
    unsigned int key_hash = table_hash(key) & (table->capacity - 1);
    if (table->avl_array[key_hash] == NULL)
        return NULL;
    table_entry *entry = avl_get_node(table->avl_array[key_hash], table_hash(key));
    while (entry != NULL && strcmp(key, entry->key) != 0)
        entry = entry->next;
    if (entry == NULL)
        return NULL;
    return entry->value;
}

void *table_remove(wtable *table, char *key) {
    --table->size;
    unsigned int key_hash = table_hash(key) & (table->capacity - 1);
    if (table->avl_array[key_hash] == NULL)
        return NULL;
    table_entry *entry = avl_get_node(table->avl_array[key_hash], table_hash(key));
    table_entry *previous = NULL;
    while (entry != NULL && strcmp(key, entry->key) != 0) {
        previous = entry;
        entry = entry->next;
    }
    if (entry == NULL)
        return NULL;
    void *result = entry->value;
    if (previous == NULL) {
        table->avl_array[key_hash] = avl_remove(table->avl_array[key_hash], table_hash(key));
        if (entry->next != NULL) {
            table->avl_array[key_hash] = avl_push(table->avl_array[key_hash], table_hash(key), entry->next);
            free(entry->key);
            free(entry);
        } else {
            avl_free(table->avl_array[key_hash]);
            table->avl_array[key_hash] = NULL;
        }

        return result;
    } else {
        previous->next = entry->next;
        free(entry->key);
        free(entry);
    }
    return result;
}

void table_keys_order(char **keys, avl_tree_node *node) {
    if(node != NULL) {
        keys[node->avl_keys_index++] = node->entry->key;
        table_keys_order(keys, node->left);
        table_keys_order(keys, node->right);
    }
}

char **table_keys(wtable *table) {
    char **keys = (char **) malloc(sizeof(char *) * table->size);
    node->avl_keys_index = 0;
    for (size_t i = 0; i < table->capacity; ++i)
        table_keys_order(keys, table->avl_array[i]);
    return keys;
}

void table_values_order(void **values, avl_tree_node *node) {
    if(node != NULL) {
        values[node->avl_values_index++] = node->entry->value;
        table_values_order(values, node->left);
        table_values_order(values, node->right);
    }
}

void **table_values(wtable *table) {
    void **values = (void **) malloc(sizeof(void *) * table->size);
    node->avl_values_index = 0;
    for (size_t i = 0; i < table->capacity; ++i)
        table_values_order(values, table->avl_array[i]);
    return values;
}

void table_free(wtable *table) {
    for (size_t i = 0; i < table->capacity; ++i)
        avl_free(table->avl_array[i]);
    free(table);
}

void table_avl_copy(wtable *table, avl_tree_node *node) {
    if (node != NULL) {
        table_push_entry(table, node->entry);
        table_avl_copy(table, node->left);
        table_avl_copy(table, node->right);
        if (node->left == NULL && node->right == NULL)
            free(node);
    }
}

void table_push_entry(wtable *table, table_entry *entry) {
    ++table->size;
    if (table->size >= (size_t) ((float) table->capacity * TABLE_DEFAULT_LODER))
        table_resize(table);
    unsigned int key_hash = entry->hash & (table->capacity - 1);
    if (table->avl_array[key_hash] == NULL) {
        table->avl_array[key_hash] = new_avl_node(entry->hash, entry);
    } else {
        table_entry *copy_entry = avl_get_node(table->avl_array[key_hash], entry->hash);
        if (copy_entry == NULL)
            table->avl_array[key_hash] = avl_push(table->avl_array[key_hash], entry->hash, entry);
        else {
            table_entry *previous = NULL;
            while (copy_entry != NULL && strcmp(entry->key, copy_entry->key) != 0) {
                previous = copy_entry;
                copy_entry = copy_entry->next;
            }
            if (previous == NULL) {
                if (strcmp(entry->key, copy_entry->key) == 0) {
                    table->avl_array[key_hash] = avl_remove(table->avl_array[key_hash], entry->hash);
                    if (copy_entry->next != NULL)
                        table->avl_array[key_hash] = avl_push(table->avl_array[key_hash], entry->hash,
                                                              copy_entry->next);
                } else {
                    table_entry *backups = copy_entry;
                    while (backups->next != NULL)
                        backups = backups->next;
                    backups->next = entry;
                }
            } else if (copy_entry != NULL) {
                free(previous->next->key);
                free(previous->next);
                previous->next = entry;
                entry->next = copy_entry->next;
            } else
                previous->next = entry;
        }
    }
}

void table_resize(wtable *table) {
    if (TABLE_MAX_CAPACITY - table->capacity < table->capacity)
        return;
    size_t backups_capacity = table->capacity;
    avl_tree_node **backups_avl_array = table->avl_array;
    table->capacity = table->capacity << 1;
    table->size = 0;
    table->avl_array = (avl_tree_node **) malloc(sizeof(avl_tree_node *) * table->capacity);
    for (size_t i = 0; i < table->capacity; ++i)
        table->avl_array[i] = NULL;
    for (size_t i = 0; i < backups_capacity; ++i)
        if (backups_avl_array[i] != NULL)
            table_avl_copy(table, backups_avl_array[i]);
    free(backups_avl_array);
}

unsigned int table_hash(char *key) {
    char *k = key;
    unsigned int hash = 0;
    while (*k != '\0') {
        hash += *k++;
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

avl_tree_node *new_avl_node(unsigned int key, table_entry *entry) {
    avl_tree_node *new = (avl_tree_node *) malloc(sizeof(avl_tree_node));
    new->key = key;
    new->entry = entry;
    new->left = NULL;
    new->right = NULL;
    new->height = 1;
    return new;
}

avl_tree_node *avl_right_rotate(avl_tree_node *y) {
    avl_tree_node *x = y->left;
    avl_tree_node *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = MAX(HEIGHT(y->left), HEIGHT(y->right)) + 1;
    x->height = MAX(HEIGHT(x->left), HEIGHT(x->right)) + 1;
    return x;
}

avl_tree_node *avl_left_rotate(avl_tree_node *x) {
    avl_tree_node *y = x->right;
    avl_tree_node *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = MAX(HEIGHT(x->left), HEIGHT(x->right)) + 1;
    y->height = MAX(HEIGHT(y->left), HEIGHT(y->right)) + 1;
    return y;
}

avl_tree_node *avl_push(avl_tree_node *node, unsigned int key, table_entry *entry) {
    if (node == NULL)
        return new_avl_node(key, entry);
    if (key < node->key)
        node->left = avl_push(node->left, key, entry);
    else if (key > node->key)
        node->right = avl_push(node->right, key, entry);
    else
        return node;
    node->height = MAX(HEIGHT(node->left), HEIGHT(node->right)) + 1;
    int balance = GET_BALANCE(node);
    if (balance > 1 && key < node->left->key)
        return avl_right_rotate(node);
    if (balance < -1 && key > node->right->key)
        return avl_left_rotate(node);
    if (balance > 1 && key > node->left->key) {
        node->left = avl_left_rotate(node->left);
        return avl_right_rotate(node);
    }
    if (balance < -1 && key < node->right->key) {
        node->right = avl_right_rotate(node->right);
        return avl_left_rotate(node);
    }
    return node;
}

avl_tree_node *avl_min_key_node(avl_tree_node *node) {
    avl_tree_node *current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

avl_tree_node *avl_remove(avl_tree_node *base, unsigned int key) {
    if (base == NULL)
        return base;
    if (key < base->key)
        base->left = avl_remove(base->left, key);
    else if (key > base->key)
        base->right = avl_remove(base->right, key);
    else {
        if ((base->left == NULL) || (base->right == NULL)) {
            avl_tree_node *temp = base->left ? base->left : base->right;
            if (temp == NULL) {
                temp = base;
                base = NULL;
            } else
                *base = *temp;
            free(temp);
        } else {
            avl_tree_node *temp = avl_min_key_node(base->right);
            base->key = temp->key;
            base->right = avl_remove(base->right, temp->key);
        }
    }
    if (base == NULL)
        return base;
    base->height = MAX(HEIGHT(base->left), HEIGHT(base->right)) + 1;
    int balance = GET_BALANCE(base);
    if (balance > 1 && GET_BALANCE(base->left) >= 0)
        return avl_right_rotate(base);
    if (balance > 1 && GET_BALANCE(base->left) < 0) {
        base->left = avl_left_rotate(base->left);
        return avl_right_rotate(base);
    }
    if (balance < -1 && GET_BALANCE(base->right) <= 0)
        return avl_left_rotate(base);
    if (balance < -1 && GET_BALANCE(base->right) > 0) {
        base->right = avl_right_rotate(base->right);
        return avl_left_rotate(base);
    }
    return base;
}

table_entry *avl_get_node(avl_tree_node *node, unsigned int key) {
    if (node == NULL)
        return NULL;
    if (key == node->key)
        return node->entry;
    if (key > node->key)
        return avl_get_node(node->right, key);
    return avl_get_node(node->left, key);
}

void avl_free(avl_tree_node *node) {
    if (node != NULL) {
        if (node->left != NULL)
            avl_free(node->left);
        if (node->right != NULL)
            avl_free(node->right);
        free(node->entry->key);
        free(node->entry);
        free(node);
    }
}
