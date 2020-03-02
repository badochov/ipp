#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "forests.h"

#define size(arr) sizeof(arr)/sizeof(arr[0])

void logError() {
  fprintf(stderr, "ERROR\n");
}

bool validateString(const char *s) {
  char *t;
  for (t = s; *t != '\0'; t++) {
    if (*t < 33) {
      return false;
    }
  }
  return true;
}

int main() {
  char *buffer;
  char *command = NULL;
  char *forest = NULL;
  char *tree = NULL;
  char *animal = NULL;
  char *more = NULL;
  unsigned long len;
  unsigned short count;
  FILE *file;
  BST *world = NULL;
  bool validationStatus;
  bool checkOutcome;
  while (getline(&buffer, &len, stdin) != -1) {
    file = fmemopen(buffer, len, "r");
    count = fscanf(file, "%s%s%s%s%s", command, forest, tree, animal, more);
    if (count == 0 || command[0] == '#') {
      continue;
    }
    validationStatus = false;
    switch (count) {
      case 1:break;
      case 4:validationStatus = validationStatus || validateString(animal);
      case 3:validationStatus = validationStatus || validateString(tree);
      case 2:validationStatus = validationStatus || validateString(forest);
        break;
      default:logError();
        continue;

    }
    if (strcmp(command, "ADD") == 0) {
      switch (count) {
        case 2: addForest(world, forest);
          break;
        case 3: addTree(world, forest, tree);
          break;
        case 4: addAnimal(world, forest, tree, animal);
          break;
        default:logError();
          continue;
      }
      fprintf(stdout, "OK\n");
    } else if (strcmp(command, "DEL") == 0) {
      switch (count) {
        case 1: deleteTree(world);
          break;
        case 2: removeForest(world, forest);
          break;
        case 3: removeTree(world, forest, tree);
          break;
        case 4: removeAnimal(world, forest, tree, animal);
          break;
        default:logError();
          continue;
      }
      fprintf(stdout, "OK\n");
    } else if (strcmp(command, "PRINT") == 0) {
      switch (count) {
        case 1: printForests(world);
          break;
        case 2: printTrees(world, forest);
          break;
        case 3: printAnimals(world, forest, tree);
          break;
        default:logError();
          continue;
      }
    } else if (strcmp(command, "CHECK") == 0) {
      switch (count) {
        case 2: checkOutcome = checkForest(world, forest);
          break;
        case 3: checkOutcome = checkTree(world, forest, tree);
          break;
        case 4: checkOutcome = checkAnimal(world, forest, tree, animal);
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

  return 0;
}
