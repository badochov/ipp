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
  char **args = malloc(sizeof(char *) * 4);
  if (args == NULL) {
    exit(1);
  }
  for (int i = 0; i < 4; i++) {
    args[i] = malloc(sizeof(char));
    if (args[i] == NULL) {
      exit(1);
    }
  }
  unsigned short count;
  short readOutcome;
  BST *world = NULL;
  bool validationStatus;
  bool checkOutcome;
  while (true) {
    for (int i = 0; i < 4; i++) {
      free(args[i]);
      args[i] = NULL;
    }
    count = 0;
    readOutcome = 0;
    for (int i = 0; i < 5; i++) {
      if (i == 0) {
        readOutcome = readCommand(&args[i]);
      } else if (i == 4) {
        readOutcome = readExtra();
      } else {
        readOutcome = readString(&args[i]);
      }

      if (readOutcome == -1 || readOutcome == 0 || readOutcome == 2) {
        break;
      }
      count++;
      if (readOutcome == 3) {
        break;
      }
    }

    if (readOutcome == 2) {
      if (args[0] != NULL) {
        logError();
      }
      break;
    }

    if (readOutcome != 3) {
      if (!reachEOL()) {
        if (!(readOutcome == 0 && count == 0)) {
          logError();
        }
        break;
      }
    }

    if (readOutcome == -1 || count == 5) {
      logError();
      continue;
    }

    if (count == 0) {
      continue;
    }
    validationStatus = false;

    for (int i = 0; i < count; i++) {
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
  for (int i = 0; i < 4; i++) {
    free(args[i]);
  }
  free(args);
  return 0;
}
