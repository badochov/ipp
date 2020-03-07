//
// Created by badochov on 02.03.2020.
//

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "input.h"

short readExtra() {
  char c = getc(stdin);
  while (c != '\n') {
    if (c == EOF && feof(stdin)) {
      return 2;
    }
    if (!isspace(c)) {
      if (c < 33) {
        return -1;
      }
      return 1;
    }
    c = getc(stdin);
  }
  return 0;
}

short readCommand(char **s) {
  char c = getc(stdin);
  if (c == '#') {
    *s = malloc(sizeof(char));
    **s = '#';
    return 0;
  }
  if (c == EOF && feof(stdin)) {
    return 2;
  }
  ungetc(c, stdin);
  return readString(s);
}

short readString(char **s) {
  int size = 0;
  int n = 0;
  char *temp;
  char c = getc(stdin);
  while (c != '\n') {
    if (c == EOF && feof(stdin)) {
      return 2;
    }
    if (!isspace(c)) {
      if (c < 33) {
        return -1;
      }
      if (n == 0) {
        *s = malloc(sizeof(char));
        size = 1;
      }
      (*s)[n] = c;
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
    c = getc(stdin);
  }
  (*s)[n++] = '\0';
  temp = realloc(*s, sizeof(char) * (n));
  if (temp == NULL) {
    exit(1);
  }
  *s = temp;

  if (c == '\n') {
    return 3;
  }
  return (short) (n > 1);
}

bool reachEOL() {
  while (getc(stdin) != '\n') {
    if (feof(stdin)) {
      return false;
    }
  }
  return true;
}


