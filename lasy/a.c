//
// Created by badochov on 07.03.2020.
//
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>

int main() {
  FILE *f = fopen("tests/z1/hb/hb_5.in", "r");
  unsigned char *c= NULL,d;
  while((d = getc(f)) != -1){
    printf("%d",d);
  }
}