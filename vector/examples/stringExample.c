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

bool vector_element_equals(void *elementInVector, void *elementToSearch) {
  elementToSearch = (char *)elementToSearch;
  char *elementInVec = *(char **)elementInVector;
  if (strncmp(elementToSearch, elementInVec, strlen(elementToSearch)) != 0) {
    return false;
  }
  printf("found\n");
  return true;
}

void free_str(void *element) { free(*(char **)element); }

struct vector string_vector;
char *names[] = {"nikos",  "george", "john", "mike",
                 "billie", "andrew", "steve"};
void *write_thread(void *arg) {

  for (int i = 0; i < 4; i++) {
    vector_push(&string_vector, names[i]);
  }
  return NULL;
}

void *read_thread1(void *arg) {
  for (int i = 0; i < 10; i++) {
    void *read = NULL;
    size_t size = vector_get_size(&string_vector);
    if (size > 0) {
      size_t index = i % size; // wrap safely
      if (vector_get(&string_vector, index, &read) == OK) {
        printf("[Reader] index %zu -> %s\n", index, (char *)read);
      }
    }
    if (read)
      free(read);
  }
  return NULL;
}

void *write_thread2(void *arg) {

  for (int i = 4; i < 6; i++) {
    vector_push(&string_vector, names[i]);
  }
  return NULL;
}

int main() {

  vector_init(&string_vector, sizeof(char *), copy_str, free_str, copy_str_get,
              vector_element_equals);

  pthread_t writer1, writer2, reader1, reader2;

  pthread_create(&writer1, NULL, write_thread, NULL);
  pthread_create(&writer2, NULL, write_thread2, NULL);
  pthread_create(&reader1, NULL, read_thread1, NULL);
  pthread_create(&reader2, NULL, read_thread1, NULL);

  pthread_join(writer1, NULL);
  pthread_join(writer2, NULL);
  pthread_join(reader1, NULL);
  pthread_join(reader2, NULL);

  vector_free(&string_vector);

  return 0;
}
