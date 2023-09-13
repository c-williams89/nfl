#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <stdint.h>

typedef uint64_t (*hash_f)(char *);

typedef struct hash_t hash_t;
hash_t *hash_table_create(uint32_t size, hash_f);

#endif



