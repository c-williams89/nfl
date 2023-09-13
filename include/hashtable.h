#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <stdint.h>
#include <stdbool.h>


typedef uint64_t (*hash_f)(char *);

typedef struct hash_t hash_t;
hash_t *hash_table_create(uint32_t size, hash_f);
bool hash_table_insert(hash_t *ht, char *key, void *data);
void hash_table_print(hash_t *ht);

#endif



