#include "../vector.h"
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

bool vector_element_equals(void *element_in_vector, void *element_to_search) {
  struct person *person_to_search = (struct person *)element_to_search;
  struct person *person_in_vector = *(struct person **)element_in_vector;
  if (person_in_vector->age == person_to_search->age &&
      strncmp(person_in_vector->name, person_to_search->name,
              strlen(person_to_search->name)) == 0) {
    return true;
  }
  return false;
}

int main() {

  printf("\nvector of deep copied structs\n");
  printf("-------------------------------\n");

  struct person person = {.name = "nikos", .age = 32};
  struct person person2 = {.name = "john", .age = 22};
  struct person person3 = {.name = "george", .age = 52};

  struct vector struct_vec_deep;
  // initialize vector
  vector_init(&struct_vec_deep, sizeof(struct person *), copy_struct,
              free_struct, copy_get, vector_element_equals);

  // push values
  vector_push(&struct_vec_deep, &person);
  vector_push(&struct_vec_deep, &person2);
  vector_push(&struct_vec_deep, &person3);

  printf("printing vector values: \n");

  for (size_t i = 0; i < struct_vec_deep.size; i++) {
    void *tmpElement = NULL;
    vector_get(&struct_vec_deep, i, &tmpElement);
    printf("name: %s, age: %d\n", ((struct person *)tmpElement)->name,
           ((struct person *)tmpElement)->age);
    free(((struct person *)tmpElement)->name);
    free(tmpElement);
  }

  printf("\nremove first test\n");
  void *first_person = NULL;
  vector_remove_front(&struct_vec_deep, true, &first_person);

  printf("removed: %s\n", ((struct person *)first_person)->name);
  free(((struct person *)first_person)->name);
  free(first_person);

  printf("\nvalues after removal:\n");

  for (size_t i = 0; i < struct_vec_deep.size; i++) {
    void *tmpElement = NULL;
    vector_get(&struct_vec_deep, i, &tmpElement);
    printf("name: %s, age: %d\n", ((struct person *)tmpElement)->name,
           ((struct person *)tmpElement)->age);
    free(((struct person *)tmpElement)->name);
    free(tmpElement);
  }

  printf("\nsearch test\n");

  // index that will be replaced with the found value position (if found)
  int found_index = -1;
  vector_find(&struct_vec_deep, &person3, &found_index);

  printf("found result index is: %d\n", found_index);

  void *found = NULL;
  vector_get(&struct_vec_deep, found_index, &found);

  printf("found value is the person with name: %s and age %d\n",
         ((struct person *)found)->name, ((struct person *)found)->age);

  // free vector
  vector_free(&struct_vec_deep);

  return 0;
}
