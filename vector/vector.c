#include "vector.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum VECTOR_ERRORS vector_init(struct vector *v, size_t elem_size,
                               void *copy_fn, void *free_fn, void *copy_get) {
  v->element_size = elem_size;
  v->size = 0;
  v->capacity = 10;
  v->data = malloc(elem_size * v->capacity);
  if (v->data == NULL) {
    return ALLOC_ERROR;
  }
  v->copy_fn = copy_fn;
  v->free_fn = free_fn;
  v->copy_get = copy_get;
  pthread_rwlock_init(&v->lock, NULL);
  return OK;
}

static enum VECTOR_ERRORS expandCapacity(struct vector *v) {
  v->capacity *= 2;
  void *new_realloc = realloc(v->data, v->capacity * v->element_size);
  if (!new_realloc) {
    return ALLOC_ERROR;
  }
  v->data = new_realloc;
  return OK;
}
enum VECTOR_ERRORS vector_remove_by_index(struct vector *v, size_t index) {
  pthread_rwlock_wrlock(&v->lock); // writers lock
  if (index < 0 || index >= v->size) {
    pthread_rwlock_unlock(&v->lock);
    return OUT_OF_BOUNDS_ERROR;
  }
  void *to_remove = (char *)v->data + index * v->element_size;
  if (v->free_fn) {
    v->free_fn(to_remove);
  }
  for (size_t i = index + 1; i < v->size; i++) {
    memcpy((char *)v->data + ((i - 1) * v->element_size),
           (char *)v->data + i * v->element_size, v->element_size);
  }
  v->size--;
  pthread_rwlock_unlock(&v->lock);
  return OK;
}
enum VECTOR_ERRORS vector_push(struct vector *v, void *element) {
  pthread_rwlock_wrlock(&v->lock); // writers lock
  if (v->size == v->capacity) {
    enum VECTOR_ERRORS error = expandCapacity(v);
    if (error == ALLOC_ERROR) {
      pthread_rwlock_unlock(&v->lock);
      return ALLOC_ERROR;
    }
  }
  void *to_copy = element;
  if (v->copy_fn) {
    to_copy = v->copy_fn(element);
    if (!to_copy) {
      pthread_rwlock_unlock(&v->lock);
      return ALLOC_ERROR;
    }
    memcpy((char *)v->data + (v->size * v->element_size), &to_copy,
           v->element_size);
  } else {
    memcpy((char *)v->data + (v->size * v->element_size), to_copy,
           v->element_size);
  }
  v->size++;
  pthread_rwlock_unlock(&v->lock);
  return OK;
}
enum VECTOR_ERRORS advanced_get(struct vector *v, size_t index, void **result) {
  pthread_rwlock_wrlock(&v->lock);
  if (v->size == 0 || index >= v->size) {
    pthread_rwlock_unlock(&v->lock);
    return OUT_OF_BOUNDS_ERROR;
  }
  if (v->copy_get) {
    *result = v->copy_get((char *)v->data + (index)*v->element_size);
  } else {
    *result = malloc(v->element_size);

    memcpy(*result, (char *)v->data + (index)*v->element_size, v->element_size);
  }

  if (index > 0) {
    char *prev = (char *)v->data + (index - 1) * v->element_size;
    char *next = (char *)v->data + (index)*v->element_size;

    unsigned char tmp[v->element_size];
    memcpy(tmp, prev, v->element_size);
    memcpy(prev, next, v->element_size);
    memcpy(next, tmp, v->element_size);
  }
  pthread_rwlock_unlock(&v->lock);
  return OK;
}
enum VECTOR_ERRORS rotate_left(struct vector *v) {
  pthread_rwlock_wrlock(&v->lock);
  if (v->size < 2) {
    pthread_rwlock_unlock(&v->lock);
    return OK;
  }
  void *first = malloc(v->element_size);

  if (!first) {
    pthread_rwlock_unlock(&v->lock);
    return ALLOC_ERROR;
  }
  memcpy(first, (char *)v->data, v->element_size);
  for (size_t i = 0; i < v->size - 1; i++) {
    memcpy((char *)v->data + (i)*v->element_size,
           (char *)v->data + (i + 1) * v->element_size, v->element_size);
  }
  memcpy((char *)v->data + (v->size - 1) * v->element_size, first,
         v->element_size);

  free(first);
  pthread_rwlock_unlock(&v->lock);
  return OK;
}
enum VECTOR_ERRORS rotate_right(struct vector *v) {
  pthread_rwlock_wrlock(&v->lock);
  if (v->size < 2) {
    pthread_rwlock_unlock(&v->lock);
    return OK;
  }
  void *last_copy = malloc(v->element_size);
  if (!last_copy) {
    pthread_rwlock_unlock(&v->lock);
    return ALLOC_ERROR;
  }
  void *last = (char *)v->data + (v->size - 1) * v->element_size;
  memcpy(last_copy, last, v->element_size);
  for (size_t i = v->size - 1; i > 0; i--) {
    memcpy((char *)v->data + (i)*v->element_size,
           (char *)v->data + (i - 1) * v->element_size, v->element_size);
  }
  memcpy((char *)v->data, last_copy, v->element_size);
  free(last_copy);
  pthread_rwlock_unlock(&v->lock);
  return OK;
}

void vector_free(struct vector *v) {
  pthread_rwlock_wrlock(&v->lock);
  if (v->free_fn) {
    for (size_t i = 0; i < v->size; i++) {
      v->free_fn((char *)v->data + i * v->element_size);
    }
  }
  free(v->data);
  v->size = 0;
  v->capacity = 0;
  pthread_rwlock_unlock(&v->lock);
  pthread_rwlock_destroy(&v->lock);
}

enum VECTOR_ERRORS vector_get(struct vector *v, size_t index, void **result) {
  pthread_rwlock_rdlock(&v->lock);
  if (index >= v->size || index < 0) {
    pthread_rwlock_unlock(&v->lock);
    return OUT_OF_BOUNDS_ERROR;
  }
  void *found = (char *)v->data + (index * v->element_size);
  if (v->copy_get) {
    *result = v->copy_get(found);
  } else {
    *result = malloc(v->element_size);
    if (!*result) {
      pthread_rwlock_unlock(&v->lock);
      return ALLOC_ERROR;
    }
    memcpy(*result, found, v->element_size);
  }
  pthread_rwlock_unlock(&v->lock);
  return OK;
}
