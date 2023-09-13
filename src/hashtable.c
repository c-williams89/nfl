#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include "hashtable.h"

#define LOAD(x) ((x * 3) / 4)

enum { PTR_SIZE = 8 };

typedef struct entry_t {
        void *data;
        bool b_to_delete;
} entry_t;

typedef struct hash_t {
        entry_t **entries;
        hash_f hash_function;
        uint32_t max_cap;
        uint32_t curr_size;
}hash_t;

hash_t *hash_table_create(uint32_t size, hash_f hf) {
        hash_t *table = NULL;
        
        if (!hf) {
                fprintf(stderr, "hash_table_create: Invalid hash_f - NULL\n");
                goto EXIT;
        }
        
        if (size < 1) {
                fprintf(stderr, "hash_table_create: Invalid size - Must be positive integer\n");
                goto EXIT;
        }

        table = calloc(1, sizeof(*table));
        if (!table) {
                perror("hash_table_create");
                errno = 0;
                goto EXIT;
        }

        table->max_cap = size;
        table->hash_function = hf;
        table->entries = calloc(table->max_cap, sizeof(entry_t *));
        if (!table->entries) {
                perror("hash_table_create");
                errno = 0;
                free(table);
                table = NULL;
        }

EXIT:
        return table;
}