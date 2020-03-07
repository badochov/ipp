//
// Created by badochov on 02.03.2020.
//

#include "input.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
short readString(char **in, char **s) {
  int size = 1;
  int n = 0;
  while (**in != '\0') {
    if (!isspace(**in)) {
      if (**in < 33) {
        return -1;
      }
      (*s)[n] = **in;
      n++;
      if (n == size) {
        size *= 2;
        *s = realloc(*s, sizeof(char) * size);
      }
    } else if (n > 0) {
      break;
    }
    (*in)++;
  }
  (*s)[n++] = '\0';
  char *temp = realloc(*s, sizeof(char) * (n));
  if (temp != NULL) {
    *s = temp;
  }
  return (short) (n > 1);
}
