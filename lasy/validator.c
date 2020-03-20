//
// Created by badochov on 07.03.2020.
//
#include <stdlib.h>
#include <string.h>

#include "validator.h"
#include "util.h"

Validator *initValidator() {
  Validator *v = alloc(sizeof(Validator));
  if (v == NULL) {
    exit(1);
  }
  v->err = v->c = v->b = false;
  return v;
}

Validator *validateLine(Line *line) {
  Validator *v = initValidator();
  if (line->eof) {
    if (line->args[0] != NULL && strcmp(line->args[0], "#") != 0) {
      v->err = true;
    }
    v->b = true;
  } else if (line->size == 5 || line->ill) {
    v->err = v->c = true;
  } else if (line->size == 0) {
    v->c = true;
  }

  return v;
}
bool isAoArgumentsOk(size_t argCount, size_t minArgCount, size_t maxArgCount) {
  return argCount >= minArgCount && argCount <= maxArgCount;
}

bool validateAdd(size_t argCount, size_t maxArgs) {
  return isAoArgumentsOk(argCount, 1, maxArgs);
}

bool validateDel(size_t argCount, size_t maxArgs) {
  return isAoArgumentsOk(argCount, 0, maxArgs);
}

bool validatePrint(size_t argCount, size_t maxArgs) {
  return isAoArgumentsOk(argCount, 0, maxArgs);
}

bool validateCheck(char **args, size_t argCount, size_t maxArgs) {
  bool argsOk = isAoArgumentsOk(argCount, 1, maxArgs);
  if (argsOk) {
    return strcmp(args[argCount - 1], "*") != 0;
  }
  return argsOk;
}

void freeValidator(Validator *v) {
  if (v == NULL) {
    return;
  }
  free(v);
}


