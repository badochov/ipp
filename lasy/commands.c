//
// Created by badochov on 01.03.2020.
//

#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "commands.h"
#include "validator.h"

/**
 * Wypisuje klucz danego drzewa
 * @param node drzewo którego klucz ma zostać wypisany
 */
void printNode(BST node, FILE *out) {
  fprintf(out, "%s\n", node->key);
}

void print(BST node, char **keys, size_t keysSize, FILE *out) {
  if (keysSize == 0) {
    traverseTreeInOrder(node, printNode, out);
    return;
  }
  BST n = getNode(node, keys[0]);
  if (n == NULL) {
    return;
  }
  print(n->children, keys + 1, keysSize - 1, out);
}

BST add(BST node, char **keys, size_t keysSize) {
  if (keysSize > 1) {
    if (node == NULL) {
      node = init(keys[0]);
    }
    int cmp = strcmp(keys[0], node->key);
    if (cmp == 0) {
      node->children = add(node->children, keys + 1, keysSize - 1);
    } else if (cmp < 0) {
      node->left = add(node->left, keys, keysSize);
    } else {
      node->right = add(node->right, keys, keysSize);
    }
  } else {
    node = addNode(node, keys[0]);
  }
  return node;
}

BST del(BST node, char **keys, size_t keysSize) {
  if (node == NULL) {
    return node;
  }
  if (keysSize > 1) {
    int cmp = strcmp(keys[0], node->key);
    if (cmp == 0) {
      node->children = del(node->children, keys + 1, keysSize - 1);
    } else if (cmp < 0) {
      node->left = del(node->left, keys, keysSize);
    } else {
      node->right = del(node->right, keys, keysSize);
    }
  } else if (keysSize == 1) {
    node = removeNode(node, keys[0]);
  } else if (keysSize == 0) {
    node = deleteTree(node);
  }
  return node;
}

bool check(BST node, char **keys, size_t keysSize) {
  if (node == NULL) {
    return false;
  }
  if (keysSize > 1) {
    if (strcmp(keys[0], "*") == 0) {
      return check(node->left, keys, keysSize)
          || check(node->children, keys + 1, keysSize - 1)
          || check(node->right, keys, keysSize);
    } else {
      BST n = getNode(node, keys[0]);
      if (n == NULL || n->children == NULL) {
        return false;
      }
      return check(n->children, keys + 1, keysSize - 1);
    }
  } else {
    return getNode(node, keys[0]) != NULL;
  }
}

void logError(FILE *f) {
  fprintf(f, "ERROR\n");
}

BST executeDel(Line *line, BST node, FILE *out, FILE *err) {
  if (validateDel(line->size - 1, 3)) {
    fprintf(out, "OK\n");
    return del(node, line->args + 1, line->size - 1);
  } else {
    logError(err);
  }
  return node;
}
BST executeAdd(Line *line, BST node, FILE *out, FILE *err) {
  if (validateAdd(line->size - 1, 3)) {
    fprintf(out, "OK\n");
    return add(node, line->args + 1, line->size - 1);
  } else {
    logError(err);
  }
  return node;
}
void executePrint(Line *line, BST node, FILE *out, FILE *err) {
  if (validatePrint(line->size - 1, 2)) {
    print(node, line->args + 1, line->size - 1, out);
  } else {
    logError(err);
  }
}
void executeCheck(Line *line, BST node, FILE *out, FILE *err) {
  if (validateCheck(line->args + 1, line->size - 1, 3)) {
    if (check(node, line->args + 1, line->size - 1)) {
      fprintf(out, "YES\n");
    } else {
      fprintf(out, "NO\n");
    }
  } else {
    logError(err);
  }
}

BST executeCommand(Line *line, BST node, FILE *out, FILE *err) {
  char *command = line->args[0];
  if (strcmp(command, "ADD") == 0) {
    node = executeAdd(line, node, out, err);
  } else if (strcmp(command, "DEL") == 0) {
    node = executeDel(line, node, out, err);
  } else if (strcmp(command, "PRINT") == 0) {
    executePrint(line, node, out, err);
  } else if (strcmp(command, "CHECK") == 0) {
    executeCheck(line, node, out, err);
  } else {
    logError(err);
  }
  return node;
}
