#include "../vector.h"
#include <stdio.h>
#include <string.h>

void *copy_str(void *element) {
  char *str_element = (char *)element;
  char *new_str = (char *)malloc(strlen(str_element) + 1);
  strncpy(new_str, str_element, strlen(str_element));
  new_str[strlen(str_element)] = '\0';
  return new_str;
}

void *copy_str_get(void *element) {
  char *str_element = *(char **)element;
  char *new_str = (char *)malloc(strlen(str_element) + 1);
  strncpy(new_str, str_element, strlen(str_element));
  new_str[strlen(str_element)] = '\0';
  return new_str;
}

void free_str(void *element) { free(*(char **)element); }

int main() {
  struct vector string_vector;
  vector_init(&string_vector, sizeof(char *), copy_str, free_str, copy_str_get);

  vector_push(&string_vector, "nikos");
  vector_push(&string_vector, "george");
  vector_push(&string_vector, "john");
  vector_push(&string_vector, "steve");
  vector_push(&string_vector, "michael");
  vector_push(&string_vector, "jim");

  for (size_t i = 0; i < string_vector.size; i++) {
    void *str = NULL;
    vector_get(&string_vector, i, &str);
    printf("string %lu: %s\n", i, (char *)str);
    free(str);
  }
  printf("remove first\n");
  void *firstName = NULL;
  remove_front(&string_vector, true, &firstName);
  for (size_t i = 0; i < string_vector.size; i++) {
    void *str = NULL;
    vector_get(&string_vector, i, &str);
    printf("string %lu: %s\n", i, (char *)str);
    free(str);
  }

  printf("name removed is: %s\n", (char *)firstName);
  free(firstName);

  printf("\ndelete test\n");
  vector_remove_by_index(&string_vector, 4);
  for (size_t i = 0; i < string_vector.size; i++) {
    void *str = NULL;
    vector_get(&string_vector, i, &str);
    printf("string %lu: %s\n", i, (char *)str);
    free(str);
  }

  printf("\nshift left test\n");
  rotate_left(&string_vector);
  for (size_t i = 0; i < string_vector.size; i++) {
    void *str = NULL;
    vector_get(&string_vector, i, &str);
    printf("string %lu: %s\n", i, (char *)str);
    free(str);
  }

  printf("\nshift right test\n");
  rotate_right(&string_vector);
  for (size_t i = 0; i < string_vector.size; i++) {
    void *str = NULL;
    vector_get(&string_vector, i, &str);
    printf("string %lu: %s\n", i, (char *)str);
    free(str);
  }

  printf("\nadvanced get\n");

  void *str = NULL;
  advanced_get(&string_vector, 1, &str);
  printf("found: %s\n", (char *)str);
  free(str);

  for (size_t i = 0; i < string_vector.size; i++) {
    void *str = NULL;
    vector_get(&string_vector, i, &str);
    printf("string %lu: %s\n", i, (char *)str);
    free(str);
  }
  vector_free(&string_vector);

  return 0;
}
