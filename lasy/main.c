#define  _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "forests.h"
#include "input.h"

void logError() {
  fprintf(stderr, "ERROR\n");
}

bool validateString(char *s) {
  char *t;
  for (t = s; *t != '\0'; t++) {
    if (*t < 33) {
      return false;
    }
  }
  return true;
}

int main() {
  char *buffer = NULL;
  char *h = NULL;
  char **args = malloc(sizeof(char *) * 5);
  for (int i = 0; i < 5; i++) {
    args[i] = malloc(sizeof(char));
  }
  size_t len;
  unsigned short count;
  short readOutcome;
  BST *world = NULL;
  bool validationStatus;
  bool checkOutcome;
  while (getline(&buffer, &len, stdin) != -1) {
    for (int i = 0; i < 5; i++) {
      char *temp = realloc(args[i], sizeof(char));
      if (temp) {
        args[i] = temp;
      }
    }
    count = 0;
    readOutcome = 0;
    h = buffer;
    for (int i = 0; i < 5; i++) {
      readOutcome = readString(&h, &args[i]);
//      printf("%s ", args[i]);
      if (readOutcome == -1 || readOutcome == 0) {
        break;
      }
      if (i == 0) {
        if (args[0][0] == '#') {
          break;
        }
      }
      count++;
    }
    if (readOutcome == -1) {
      logError();
      continue;
    }
    if (count == 0 || args[0][0] == '#') {
      continue;
    }
    validationStatus = count == 1;
    if (count == 5) {
      logError();
      continue;
    }
    for (int i = 1; i < count; i++) {
      validationStatus = validationStatus || validateString(args[i]);
    }
    if (!validationStatus) {
      logError();
      continue;
    }
    if (strcmp(args[0], "ADD") == 0) {
      switch (count) {
        case 2: addForest(&world, args[1]);
          break;
        case 3: addTree(&world, args[1], args[2]);
          break;
        case 4: addAnimal(&world, args[1], args[2], args[3]);
          break;
        default:logError();
          continue;
      }
      fprintf(stdout, "OK\n");
    } else if (strcmp(args[0], "DEL") == 0) {
      switch (count) {
        case 1: deleteTree(&world);
          break;
        case 2: removeForest(&world, args[1]);
          break;
        case 3: removeTree(&world, args[1], args[2]);
          break;
        case 4: removeAnimal(&world, args[1], args[2], args[3]);
          break;
        default:logError();
          continue;
      }
      fprintf(stdout, "OK\n");
    } else if (strcmp(args[0], "PRINT") == 0) {
      switch (count) {
        case 1: printForests(world);
          break;
        case 2: printTrees(world, args[1]);
          break;
        case 3: printAnimals(world, args[1], args[2]);
          break;
        default:logError();
          continue;
      }
    } else if (strcmp(args[0], "CHECK") == 0) {
      if (count == 1 || strcmp(args[count - 1], "*") == 0) {
        logError();
        continue;
      }
      switch (count) {
        case 2: checkOutcome = checkForest(world, args[1]);
          break;
        case 3: checkOutcome = checkTree(world, args[1], args[2]);
          break;
        case 4: checkOutcome = checkAnimal(world, args[1], args[2], args[3]);
          break;
        default:logError();
          continue;
      }
      if (checkOutcome) {
        fprintf(stdout, "YES\n");
      } else {
        fprintf(stdout, "NO\n");
      }
    } else {
      logError();
      continue;
    }
  }
  deleteTree(&world);
//  free(h);
  free(buffer);
  for (int i = 0; i < 5; i++) {
    free(args[i]);
  }
  free(args);
  return 0;
}
