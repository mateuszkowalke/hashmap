#include "hashmap.h"
#include <stddef.h>
#include <string.h>
#include <strings.h>

typedef struct entry {
  char *key;
  void *obj;
  struct entry *next;
} entry;

typedef struct _hashmap {
  uint32_t size;
  hashfunc *hf;
  entry **entries;
} hashmap;

static size_t hash(hashmap *hm, const char *key) {
  return hm->hf(key, strlen(key)) % hm->size;
}

// returns pointer to next element if exists
static entry *free_entry(entry *e) {
  entry *tmp;
  free(e->key);
  tmp = e->next;
  free(e);
  return tmp;
}

hashmap *hashmap_create(uint32_t size, hashfunc *hf) {
  hashmap *hm = malloc(sizeof(hashmap));
  hm->size = size;
  hm->hf = hf;
  hm->entries = calloc(sizeof(entry *), size);
  return hm;
}

void hashmap_destroy(hashmap *hm) {
  for (int i = 0; i < hm->size; i++) {
    entry *e = hm->entries[i];
    while (e != NULL) {
      e = free_entry(e);
    }
  }
  free(hm->entries);
  free(hm);
}

void hashmap_print(hashmap *hm) {
  printf("Hashmap:\n");
  for (int i = 0; i < hm->size; i++) {
    entry *e = hm->entries[i];
    if (NULL == e) {
      printf("\t%i\t----\n", i);
      continue;
    }
    printf("\t%i\t", i);
    while (NULL != e) {
      printf("\"%s\"(%p) - ", e->key, e->obj);
      e = e->next;
    }
    printf("\n");
  }
  printf("End\n\n");
}

// hashmap_insert makes it's own copy of key, so the argument passed can be freed at will by the caller
// caller takes the responsibility for freeing the obj argument after deleting it from hashmap and also making sure it lives long enough (while the obj is in hashmap)
bool hashmap_insert(hashmap *hm, const char *key, void *obj) {
  if (NULL == hm || NULL == key || NULL == obj ||
      hashmap_lookup(hm, key) != NULL)
    return false;
  size_t h = hash(hm, key);
  entry *e = malloc(sizeof(entry));
  e->key = malloc(strlen(key) + 1);
  strcpy(e->key, key);
  e->obj = obj;
  e->next = NULL;
  e->next = hm->entries[h];
  hm->entries[h] = e;
  return true;
}

void *hashmap_lookup(hashmap *hm, const char *key) {
  if (NULL == hm || NULL == key)
    return NULL;
  size_t h = hash(hm, key);
  entry *e = hm->entries[h];
  while (NULL != e && strcmp(key, e->key) != 0) {
    e = e->next;
  }
  if (NULL == e)
    return NULL;
  return e->obj;
}

void *hashmap_delete(hashmap *hm, const char *key) {
  if (NULL == hm || NULL == key)
    return NULL;
  size_t h = hash(hm, key);
  entry *e = hm->entries[h];
  entry *prev = NULL;
  while (NULL != e && strcmp(key, e->key) != 0) {
    prev = e;
    e = e->next;
  }
  if (NULL == e)
    return NULL;
  void *result = e->obj;
  if (NULL == prev) {
    hm->entries[h] = free_entry(e);
  } else {
    prev->next = free_entry(e);
  }
  return result;
}
