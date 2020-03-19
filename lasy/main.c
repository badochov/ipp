#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "commands.h"
#include "validator.h"
#include "input.h"

int main() {
  Line *line;
  Validator *v;
  BST world = NULL;
  while (true) {
    line = readLine(stdin, 4);
    v = validateLine(line);
    if (v->err) {
      logError(stderr);
    }
    if (!(v->c || v->b || v->err)) {
      world = executeCommand(line, world, stdout, stderr);
    }
    bool b = v->b;
    freeValidator(v);
    freeLine(line);
    if (b) {
      break;
    }
  }
  //  free memory
  deleteTree(world);
  return 0;
}
