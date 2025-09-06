#include "../vector.h"
#include <stdio.h>

struct dog {
  char *name;
  int age;
};

int main() {
  struct dog dog = {.name = "rex", .age = 12};
  struct dog dog2 = {.name = "java", .age = 13};
  struct vector vec;
  vector_init(&vec, sizeof(struct dog), NULL, NULL, NULL);
  vector_push(&vec, &dog);
  vector_push(&vec, &dog2);

  for (size_t i = 0; i < vec.size; i++) {
    void *found = NULL;
    vector_get(&vec, i, &found);
    printf("dog name: %s, dog age: %d\n", ((struct dog *)found)->name,
           ((struct dog *)found)->age);
    free(found);
  }
  vector_free(&vec);

  return 0;
}
