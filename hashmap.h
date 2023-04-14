#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef uint64_t hashfunc (const char*, size_t);
typedef struct _hashmap hashmap;

hashmap *hashmap_create(uint32_t size, hashfunc *hf);
void hashmap_destroy(hashmap *hm);
void hashmap_print(hashmap *hm);
bool hashmap_insert(hashmap *hm, const char *key, void *obj);
void *hashmap_lookup(hashmap *hm, const char *key);
void *hashmap_delete(hashmap *hm, const char *key);

#endif
