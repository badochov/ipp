//
// Created by badochov on 07.03.2020.
//
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>

int main() {
  FILE *f = fopen("tests/z1/jm/jm_3.in", "r");
  char *c = NULL;
  size_t len;
  getline(&c, &len, f);
  printf("%s", c);
}