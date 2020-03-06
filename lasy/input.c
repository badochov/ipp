//
// Created by badochov on 02.03.2020.
//

#include "input.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

short readExtra(char *in) {
  char *it = in;
  while (*it != '\0') {
    if (!isspace(*it)) {
      if (*it < 33) {
        return -1;
      }
      return 1;
    }
    it++;
  }
  return 0;
}

short readCommand(char **in, char **s) {
  if (**in == '#') {
    **s = '#';
    return 0;
  }
  return readString(in, s);
}

short readString(char **in, char **s) {
  int size = 1;
  int n = 0;
  char *temp;
  while (**in != '\0') {
    if (!isspace(**in)) {
      if (**in < 33) {
        return -1;
      }
      (*s)[n] = **in;
      n++;
      if (n == size) {
        size *= 2;
        temp = realloc(*s, sizeof(char) * size);
        if (temp == NULL) {
          exit(1);
        }
        *s = temp;
      }
    } else if (n > 0) {
      break;
    }
    (*in)++;
  }
  (*s)[n++] = '\0';
  temp = realloc(*s, sizeof(char) * (n));
  if (temp == NULL) {
    exit(1);
  }
  *s = temp;

  return (short) (n > 1);
}


