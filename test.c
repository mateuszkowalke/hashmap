#include "hashmap.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE 4096

uint64_t myhashfunc(const char *name, size_t size) {
  uint64_t hash = 0;
  for (int i = 0; i < size; i++) {
    hash += name[i];
    hash = (hash * name[i]);
  }
  return hash;
}

void generate_random_guess(char *buf, size_t size) {
  for (size_t i = 0; i < size; i++) {
    buf[i] = 'a' + (rand() % 26);
  }
  buf[size - 1] = '\0';
}

int main(int argc, char *argv[]) {
  if (3 != argc) {
    printf("usage: %s <wordlist filename> <num guesses>\n", argv[0]);
    return EXIT_FAILURE;
  }

  char *filename = argv[1];
  uint32_t num_guesses = atol(argv[2]);
  const int mapsize = 1 << 17;

  hashmap *hm = hashmap_create(mapsize, myhashfunc);

  FILE *fp;
  if ((fp = fopen(filename, "r")) < 0) {
    perror("Error opening file.");
    return EXIT_FAILURE;
  }
  char buf[MAX_LINE];
  uint32_t num_words = 0;

  while (!feof(fp) && fgets(buf, MAX_LINE, fp) != NULL) {
    buf[strcspn(buf, "\n\r")] = '\0';
    hashmap_insert(hm, buf, NULL);
    num_words++;
  }

  if (fclose(fp) < 0) {
    perror("Error closing file.");
    return EXIT_FAILURE;
  }

  printf("Loaded %d words into hashmap.\n", num_words);

  /* hashmap_print(hm); */

  uint32_t good_guesses = 0;
  const int shortest_guess = 2;
  const int longest_guess = 15;
  for (int i = 0; i < num_guesses; i++) {
    generate_random_guess(buf, shortest_guess +
                                   (rand() % (longest_guess - shortest_guess)));
    printf("Guess: \"%s\"\n", buf);
    if (hashmap_lookup(hm, buf) != NULL) {
      good_guesses++;
    }
  }

  printf("%u out of %u guesses were in the hash map.\n", good_guesses,
         num_guesses);

  hashmap_destroy(hm);

  return EXIT_SUCCESS;
}
