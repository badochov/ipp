//
// Created by badochov on 03.03.2020.
//

#ifndef VECTOR__VECTOR_H_
#define VECTOR__VECTOR_H_

#include <glob.h>
#include <stdbool.h>
#define T int
typedef struct vector {
  size_t size;
  size_t max_size;
  T *arr;
} vector;

vector *init();

T at(vector *v, size_t pos);

vector *insert(vector *v, size_t pos, T val);

vector *set(vector *v, size_t pos, T val);

vector *push_back(vector *v, T val);

void swap(vector **v1, vector **v2);

vector *pop_back(vector *v);

bool is_empty(vector *v);

vector *shrink_to_fit(vector *v);

vector *clear(vector *v);

T front(vector *v);

T back(vector *v);

vector *erase(vector *v, size_t pos);


bool resize(vector **v, size_t s);

#endif //VECTOR__VECTOR_H_
