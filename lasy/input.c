//
// Created by badochov on 02.03.2020.
//

#include "input.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
short readString(char **in, char **s) {
  char *res = malloc(sizeof(char));
  int size = 1;
  int n = 0;
  while (**in != '\0') {
    if (!isspace(**in)) {
      if (**in < 33) {
        return -1;
      }
      res[n] = **in;
      n++;
      if (n == size) {
        size *= 2;
        res = realloc(res, sizeof(char) * size);
      }
    } else if (n > 0) {
      break;
    }
    (*in)++;
  }
//  free(s);
  *s = realloc(res, sizeof(char) * n);
//  free(res);
  return (short)(n > 0);
}
