#ifndef __VECTOR_H__
#define __VECTOR_H__
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#define FORV(i, vec) for (size_t i = 0; i < (vec).len; ++i)

#define DEFINE_VEC(T)                                                          \
  typedef struct {                                                             \
    T *arr;                                                                    \
    size_t len;                                                                \
    size_t cap;                                                                \
  } vector_##T;                                                                \
                                                                               \
  void vector_##T##_init(vector_##T *const);                                   \
  void vector_##T##_free(vector_##T *const);                                   \
  void vector_##T##_push(vector_##T *const, T);                                \
  void vector_##T##_insert(vector_##T *const, T, size_t);                      \
  T vector_##T##_at(vector_##T *const, size_t);                                \
  T *vector_##T##_atp(vector_##T *const, size_t);                              \
  T vector_##T##_pop(vector_##T *const);

#define IMPLEMENT_VEC(T)                                                       \
  void vector_##T##_init(vector_##T *const vec) {                              \
    vec->arr = NULL;                                                           \
    vec->len = 0;                                                              \
    vec->cap = 0;                                                              \
  }                                                                            \
                                                                               \
  void vector_##T##_free(vector_##T *const vec) { free(vec->arr); }            \
                                                                               \
  void vector_##T##_push(vector_##T *const vec, T x) {                         \
    if (vec->len >= vec->cap) {                                                \
      vec->cap = (vec->cap ? (vec->cap * 2) : 8);                              \
      vec->arr = realloc(vec->arr, vec->cap * sizeof(T));                      \
    }                                                                          \
                                                                               \
    vec->arr[vec->len] = x;                                                    \
    ++vec->len;                                                                \
  }                                                                            \
                                                                               \
  void vector_##T##_insert(vector_##T *const vec, T item, size_t pos) {        \
    if (pos > vec->len + 1)                                                    \
      return;                                                                  \
    if (pos == vec->len) {                                                     \
      vector_##T##_push(vec, item);                                            \
      return;                                                                  \
    }                                                                          \
                                                                               \
    if (vec->len >= vec->cap) {                                                \
      vec->cap = (vec->cap ? (vec->cap * 2) : 8);                              \
      vec->arr = realloc(vec->arr, vec->cap * sizeof(T));                      \
    }                                                                          \
                                                                               \
    for (size_t i = vec->len - 1; i >= pos; --i) {                             \
      vec->arr[i + 1] = vec->arr[i];                                           \
    }                                                                          \
    vec->arr[pos] = item;                                                      \
    ++vec->len;                                                                \
  }                                                                            \
                                                                               \
  T vector_##T##_at(vector_##T *const vec, size_t idx) {                       \
    return *vector_##T##_atp(vec, idx);                                        \
  }                                                                            \
                                                                               \
  T *vector_##T##_atp(vector_##T *const vec, size_t idx) {                     \
    assert(idx < vec->len);                                                    \
                                                                               \
    return &vec->arr[idx];                                                     \
  }                                                                            \
                                                                               \
  T vector_##T##_pop(vector_##T *const vec) {                                  \
    T elem = vector_##T##_at(vec, vec->len - 1);                               \
    --vec->len;                                                                \
    if (vec->cap > 8 && vec->len < vec->cap / 2) {                             \
      vec->cap /= 2;                                                           \
      vec->arr = (T *)realloc(vec->arr, vec->cap * sizeof(T));                 \
    }                                                                          \
                                                                               \
    return elem;                                                               \
  }

#endif // !__VECTOR_H__
