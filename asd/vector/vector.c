//
// Created by badochov on 03.03.2020.
//

#include <stdlib.h>
#include "vector.h"

size_t min(size_t v1, size_t v2) {
  return v1 < v2 ? v1 : v2;
}

size_t max(size_t v1, size_t v2) {
  return v1 > v2 ? v1 : v2;
}

vector *init() {
  vector *v = malloc(sizeof(vector));
  v->size = 0;
  v->max_size = 0;
  v->arr = NULL;
  return v;
}

bool resize(vector **v, size_t s) {
  T *temp;
  if ((*v)->size == 0) {
    temp = malloc(sizeof(T) * s);
  } else {
    temp = realloc((*v)->arr, sizeof(T) * s);
  }
  if (temp == NULL) {
    return false;
  }
  (*v)->arr = temp;
  (*v)->max_size = s;
  (*v)->size = min(s, (*v)->size);
  return true;
}

T at(vector *v, size_t pos) {
  if (v->size > pos) {
    return v->arr[pos];
  }
  return v->arr[-1];
}

size_t find_size(size_t size, size_t pos) {
  size = max(1, size);
  while (size < pos) {
    size *= 2;
  }
  return size;
}

vector *put(vector *v, size_t pos, T val, bool move) {
  if (pos + 1 > v->max_size) {
    size_t size = find_size(v->max_size, pos + 1);
    bool res = resize(&v, size);
    if (!res) {
      return NULL;
    }
  }
  if (move) {
    int i = v->size;
    while (i > pos) {
      v->arr[i] = v->arr[i - 1];
      i--;
    }
  }
  v->size = max(pos + 1, v->size + move);
  v->arr[pos] = val;
  return v;
}

vector *insert(vector *v, size_t pos, T val) {
  return put(v, pos, val, true);
}

vector *set(vector *v, size_t pos, T val) {
  return put(v, pos, val, false);
}

vector *push_back(vector *v, T val) {
  return set(v, v->size, val);
}

void swap(vector **v1, vector **v2) {
  vector *temp = *v1;
  *v1 = *v2;
  *v2 = temp;
}

vector *erase(vector *v, size_t pos) {
  if (v->size > pos) {
    while (pos < v->size) {
      v->arr[pos] = v->arr[pos + 1];
      pos++;
    }
    v->size--;
  }
  if (v->size <= v->max_size / 2) {
    resize(&v, v->max_size / 2);
  }
  return NULL;
}

vector *pop_back(vector *v) {
  return erase(v, v->size - 1);
}

bool is_empty(vector *v) {
  return v->size == 0;
}

vector *shrink_to_fit(vector *v) {
  if (resize(&v, v->size)) {
    return v;
  }
  return NULL;
}

vector *clear(vector *v) {
  free(v->arr);
  v = malloc(sizeof(T));
  v->size = 0;
  v->max_size = 1;
  return v;
}

T front(vector *v) {
  return v->arr[0];
}

T back(vector *v) {
  return v->arr[v->size - 1];
}

