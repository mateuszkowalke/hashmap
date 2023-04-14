#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 256
#define TABLE_SIZE 10

typedef struct {
  char name[MAX_NAME];
  int age;
  struct person *next;
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
    if (table[i] == NULL) {
      printf("\t%i\t---\n", i);
    } else {
      printf("\t%i\t", i);
      person *p = table[i];
      while (p != NULL) {
          printf("%s - ", p->name);
          p = p->next;
      }
      printf("\n");
    }
  }
}

bool insert_people_table(person *p, person **table) {
  if (p == NULL)
    return false;
  int i = hash(p->name);
  p->next = table[i];
  table[i] = p;
  return true;
}

person *lookup_people_table(const char *name, person **table) {
  int i = hash(name);
  person *p = table[i];
  while (p != NULL && strncmp(name, p->name, MAX_NAME) != 0)
    p = p->next;
  return p;
}

person *delete_from_people_table(const char *name, person **table) {
  int i = hash(name);
  person *p = table[i];
  person *prev = NULL;
  while (p != NULL && strncmp(name, p->name, MAX_NAME) != 0) {
    prev = p;
    p = p->next;
  }
  if (p == NULL)
    return NULL;
  if (prev == NULL) {
    table[i] = p->next;
    return p;
  }
  prev->next = p->next;
  return p;
}

int main(int argc, char *argv[]) {
  person *people_table[TABLE_SIZE];
  person some = {.name = "some", .age = 23};
  person some2 = {.name = "some2", .age = 31};
  person some3 = {.name = "some3", .age = 44};
  person some4 = {.name = "some4", .age = 44};
  person some5 = {.name = "some5", .age = 55};
  person some6 = {.name = "some6", .age = 44};
  init_people_table(people_table);
  insert_people_table(&some, people_table);
  insert_people_table(&some2, people_table);
  insert_people_table(&some3, people_table);
  insert_people_table(&some4, people_table);
  insert_people_table(&some5, people_table);
  insert_people_table(&some6, people_table);
  print_people_table(people_table);
  return EXIT_SUCCESS;
}
