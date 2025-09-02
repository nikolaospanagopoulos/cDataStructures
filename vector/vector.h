#pragma once
#include <pthread.h>
#include <stdlib.h>
enum VECTOR_ERRORS {
  OK = 0,
  ALLOC_ERROR,
  OUT_OF_BOUNDS_ERROR,
};

struct vector {
  size_t size;
  size_t capacity;
  void *data;
  size_t element_size;
  void *(*copy_fn)(void *element);
  void (*free_fn)(void *element);
  void *(*copy_get)(void *element);
  pthread_rwlock_t lock;
};

enum VECTOR_ERRORS advanced_get(struct vector *v, size_t index, void **result);
/**
 * Removes an element from the vector at a specified index.
 *
 * Shifts all subsequent elements one position to the left to fill the gap.
 * If a custom free function is provided, it will be called on the removed
 * element. The operation is thread-safe using a write lock.
 *
 * @param v     Pointer to the vector structure.
 * @param index Index of the element to remove (0-based).
 *
 * @return OK on success, OUT_OF_BOUNDS_ERROR if the index is invalid.
 */

enum VECTOR_ERRORS vector_remove_by_index(struct vector *v, size_t index);
/**
 * Initializes a dynamic vector.
 *
 * Allocates memory for the vector's internal storage, sets initial size
 * and capacity, and optionally assigns custom functions for copying and
 * freeing elements. Also initializes a read-write lock for thread safety.
 *
 * @param v        Pointer to the vector structure to initialize.
 * @param elem_size Size in bytes of each element to be stored.
 * @param copy_fn   Optional function pointer for deep-copying elements
 *                  when added to the vector (NULL for shallow copy).
 * @param free_fn   Optional function pointer for freeing elements
 *                  when removed from the vector or on destruction.
 * @param copy_get  Optional function pointer for deep-copying elements
 *                  when accessed (used in vector_get or advanced_get).
 *
 * @return OK on success, ALLOC_ERROR if memory allocation fails.
 */
enum VECTOR_ERRORS vector_init(struct vector *v, size_t elem_size,
                               void *copy_fn, void *free_fn, void *copy_get);
static enum VECTOR_ERRORS expandCapacity(struct vector *v);
/**
 * Rotates the elements of the vector one position to the left.
 *
 * The first element moves to the last position, and all other elements
 * shift one position to the left. No rotation is performed if the vector
 * has fewer than two elements.
 *
 * This operation is thread-safe using a write lock.
 *
 * @param v Pointer to the vector structure.
 *
 * @return OK on success, ALLOC_ERROR if memory allocation for temporary storage
 * fails.
 */
enum VECTOR_ERRORS rotate_left(struct vector *v);
/**
 * Rotates the elements of the vector one position to the right.
 *
 * The last element moves to the first position, and all other elements
 * shift one position to the right. No rotation is performed if the vector
 * has fewer than two elements.
 *
 * This operation is thread-safe using a write lock.
 *
 * @param v Pointer to the vector structure.
 *
 * @return OK on success, ALLOC_ERROR if memory allocation for temporary storage
 * fails.
 */
enum VECTOR_ERRORS rotate_right(struct vector *v);
/**
 * @brief Appends an element to the end of the vector.
 *
 * If the vector is full, its capacity is automatically expanded.
 * If a copy function (`copy_fn`) was provided at initialization,
 * the element is first duplicated using `copy_fn` before being stored.
 * Otherwise, the element's raw bytes are copied directly into the vector.
 *
 * @param v Pointer to the vector.
 * @param element Pointer to the element to be added.
 *                - If `copy_fn` is NULL, `element` must point to a block of
 *                  memory of size `v->element_size`.
 *                - If `copy_fn` is not NULL, `element` is passed to `copy_fn`
 *                  to produce a heap-allocated copy.
 *
 * @return enum VECTOR_ERRORS
 *         - OK:           Element successfully pushed.
 *         - ALLOC_ERROR:  Memory allocation failed during expansion or copy.
 */

enum VECTOR_ERRORS vector_push(struct vector *v, void *element);
/**
 * Frees all resources associated with the vector.
 *
 * If a custom free function is provided, it is called on each element
 * to release any element-specific memory. Then, the vector's internal
 * data array is freed. The vector's size and capacity are reset to 0.
 *
 * The operation is thread-safe using a write lock and also destroys
 * the vector's read-write lock.
 *
 * @param v Pointer to the vector to be freed.
 */

void vector_free(struct vector *v);
/**
 * Retrieves a copy of the element at the specified index in the vector.
 *
 * If a custom copy function (`copy_get`) is provided, it is used to create
 * the returned copy. Otherwise, a default shallow copy is allocated and
 * populated using memcpy.
 *
 * The function is thread-safe, using a read lock to allow concurrent reads.
 *
 * @param v Pointer to the vector.
 * @param index Index of the element to retrieve.
 * @param result Output pointer that will point to the copied element.
 * @return OK if successful, OUT_OF_BOUNDS_ERROR if the index is invalid,
 *         or ALLOC_ERROR if memory allocation for the copy fails.
 */

enum VECTOR_ERRORS vector_get(struct vector *v, size_t index, void **result);
