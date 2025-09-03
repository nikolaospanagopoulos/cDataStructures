#include "../vector.h"
#include <stdio.h>
#include <time.h>

int main() {

  struct vector v2;

  vector_init(&v2, sizeof(int), NULL, NULL, NULL);

  int num1 = 5;
  int num2 = 21;
  int num3 = 22;
  int num4 = 23;
  int num5 = 24;
  int num6 = 25;

  vector_push(&v2, &num1);
  vector_push(&v2, &num2);
  vector_push(&v2, &num3);
  vector_push(&v2, &num4);
  vector_push(&v2, &num5);
  vector_push(&v2, &num6);

  for (size_t i = 0; i < v2.size; i++) {
    void *tmpElement = NULL;
    vector_get(&v2, i, &tmpElement);
    printf("%d\n", *(int *)tmpElement);
    free(tmpElement);
  }
  printf("remove front\n");
  void *tmpEl = NULL;
  remove_front(&v2, true, &tmpEl);
  for (size_t i = 0; i < v2.size; i++) {
    void *tmpElement = NULL;
    vector_get(&v2, i, &tmpElement);
    printf("%d\n", *(int *)tmpElement);
    free(tmpElement);
  }

  printf("removed element: %d\n", *(int *)tmpEl);
  free(tmpEl);

  printf("rotate_left \n");
  void *returned = NULL;
  void *result = NULL;
  // advanced_get(&v2, 5, &returned);
  // advanced_get(&v2, 4, &result);

  // printf("found number: %d\n", *(int *)returned);
  // printf("found number2: %d\n", *(int *)result);

  // free(returned);
  // free(result);

  // advanced_get(&v2, 4);
  //    rotate_right(&v2);
  rotate_left(&v2);
  rotate_left(&v2);
  rotate_right(&v2);
  rotate_right(&v2);
  for (size_t i = 0; i < v2.size; i++) {
    void *tmpElement = NULL;
    vector_get(&v2, i, &tmpElement);
    printf("%d\n", *(int *)tmpElement);
    free(tmpElement);
  }
  vector_free(&v2);

  return 0;
}
