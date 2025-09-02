#include "vector.h"
#include <stdio.h>
#include <string.h>

struct person {
  char *name;
  int age;
};
void *copy_struct(void *element) {
  struct person *to_copy = (struct person *)element;
  struct person *new_person = (struct person *)malloc(sizeof(struct person));
  new_person->age = to_copy->age;
  new_person->name = (char *)malloc(strlen(to_copy->name) + 1);
  strncpy(new_person->name, to_copy->name, strlen(to_copy->name));
  new_person->name[strlen(to_copy->name)] = '\0';
  return new_person;
}
void *copy_get(void *element) {
  struct person *to_copy = *(struct person **)element;
  struct person *new_person = (struct person *)malloc(sizeof(struct person));
  new_person->age = to_copy->age;
  new_person->name = (char *)malloc(strlen(to_copy->name) + 1);
  strncpy(new_person->name, to_copy->name, strlen(to_copy->name));
  new_person->name[strlen(to_copy->name)] = '\0';
  return new_person;
}

void free_struct(void *element) {
  struct person *to_free = *(struct person **)element;
  free(to_free->name);
  free(to_free);
}

int main() {

  printf("struct deep copies vec\n");

  struct person person = {.name = "nikos", .age = 32};
  struct person person2 = {.name = "john", .age = 22};
  struct person person3 = {.name = "george", .age = 52};

  struct vector struct_vec_deep;
  vector_init(&struct_vec_deep, sizeof(struct person *), copy_struct,
              free_struct, copy_get);
  vector_push(&struct_vec_deep, &person);
  vector_push(&struct_vec_deep, &person2);
  vector_push(&struct_vec_deep, &person3);

  // vector_remove_by_index(&struct_vec_deep, 1);

  for (size_t i = 0; i < struct_vec_deep.size; i++) {
    void *tmpElement = NULL;
    vector_get(&struct_vec_deep, i, &tmpElement);
    printf("name: %s, age: %d\n", ((struct person *)tmpElement)->name,
           ((struct person *)tmpElement)->age);
    free(tmpElement);
  }

  vector_free(&struct_vec_deep);

  return 0;
}
