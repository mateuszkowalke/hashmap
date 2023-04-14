#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 256
#define TABLE_SIZE 10
#define DELETED_PERSON (person *)0xFFFFFFFFFFFFFUL

typedef struct {
  char name[MAX_NAME];
  int age;
} person;

unsigned int hash(const char *name) {
  int len = strnlen(name, MAX_NAME);
  int hash = 0;
  for (int i = 0; i < len; i++) {
    hash += name[i];
    hash = (hash * name[i]) % TABLE_SIZE;
  }
  return hash;
}

void init_people_table(person **table) {
  for (int i = 0; i < TABLE_SIZE; i++) {
    table[i] = NULL;
  }
}

void print_people_table(person **table) {
  for (int i = 0; i < TABLE_SIZE; i++) {
    if (table[i] == NULL || table[i] == DELETED_PERSON) {
      printf("\t%i\t---\n", i);
    } else {
      printf("\t%i\t%s\n", i, table[i]->name);
    }
  }
}

bool insert_people_table(person *p, person **table) {
  if (p == NULL)
    return false;
  int index = hash(p->name);
  int tmpIndex;
  for (int i = 0; i < TABLE_SIZE; i++) {
    tmpIndex = (index + i) % TABLE_SIZE;
    if (table[tmpIndex] == NULL || table[tmpIndex] == DELETED_PERSON) {
      table[tmpIndex] = p;
      return true;
    }
  }
  return false;
}

person *lookup_people_table(const char *name, person **table) {
  int index = hash(name);
  person *p;
  int tmpIndex;
  for (int i = 0; i < TABLE_SIZE; i++) {
    tmpIndex = (index + i) % TABLE_SIZE;
    p = table[tmpIndex];
    if (p == NULL)
      return NULL;
    if (p == DELETED_PERSON)
      continue;
    if (strncmp(p->name, name, MAX_NAME) == 0) {
      return p;
    }
  }
  return NULL;
}

person *delete_from_people_table(const char *name, person **table) {
  int index = hash(name);
  person *p;
  int tmpIndex;
  for (int i = 0; i < TABLE_SIZE; i++) {
    tmpIndex = (index + i) % TABLE_SIZE;
    p = table[tmpIndex];
    if (p == NULL)
      return NULL;
    if (p == DELETED_PERSON)
      continue;
    if (strncmp(p->name, name, MAX_NAME) == 0) {
      table[tmpIndex] = DELETED_PERSON;
      return p;
    }
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  person *people_table[TABLE_SIZE];
  person some = {.name = "some", .age = 23};
  person some2 = {.name = "some2", .age = 31};
  person some3 = {.name = "some3", .age = 44};
  init_people_table(people_table);
  insert_people_table(&some, people_table);
  insert_people_table(&some2, people_table);
  insert_people_table(&some3, people_table);
  print_people_table(people_table);
  return EXIT_SUCCESS;
}
