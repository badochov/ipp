//
// Created by badochov on 08.03.2020.
//

#include <stdlib.h>

#include "util.h"

void *resize(void *arr, size_t len) {
  void *temp = realloc(arr, sizeof(*arr) * len);
  if (temp == NULL) {
    exit(1);
  }
  return temp;
}

void *alloc(size_t size) {
  void *temp = malloc(size);
  if (temp == NULL) {
    exit(1);
  }
  return temp;
}