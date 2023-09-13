#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hashtable.h"

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

hash_t *hash_table_create(uint32_t size, hash_f hash_func) {
        return -1;
}