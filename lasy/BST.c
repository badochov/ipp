#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "BST.h"
//
// Created by badochov on 01.03.2020.
//

BST *init(char *value) {
  BST *node = malloc(sizeof(BST));
  node->value = value;
  node->right = NULL;
  node->left = NULL;
  node->children = NULL;
  return node;
}

BST *addNode(BST *node, char *val) {
  if (node == NULL) {
    return init(val);
  }
  int cmp = strcmp(val, node->value);
  if (cmp > 0) {
    node->right = addNode(node->right, val);
  } else if (cmp < 0) {
    node->left = addNode(node->left, val);
  }
  return node;
}

BST *getNode(BST *node, char *val) {
  if (node == NULL) {
    return NULL;
  }
  int cmp = strcmp(val, node->value);
  if (cmp == 0) {
    return node;
  } else if (cmp < 0) {
    return getNode(node->left, val);
  } else {
    return getNode(node->right, val);
  }
}

bool checkNode(BST *node, char *val) {
  return getNode(node, val) == NULL;
}

bool deleteTree(BST *node) {
  if (node == NULL) {
    return true;
  }
  if (deleteTree(node->left) && deleteTree(node->children) && deleteTree(node->right)) {
    free(node);
    node = NULL;
    return true;
  }
  return false;
}

// procedura pomocnicza usuwająca node'a
BST *deleteNode(BST *node) {
  if (node == NULL) {
    return NULL;
  }
  deleteTree(node->children);
  if (node->left == NULL) {
    return node->right;
  }
  node->value = node->left->value;
  node->left = deleteNode(node->left);
  return node;
}

BST *removeNode(BST *node, char *val) {
  BST *n = getNode(node, val);
  if (n == NULL) {
    return NULL;
  }
  deleteNode(n);

  return node;
}

BST *addChild(BST *node, char *val, char *child) {
  BST *n = getNode(node, val);
  if (n == NULL) {
    return NULL;
  }
  addNode(n->children, child);
  return node;
}

BST *getChild(BST *node, char *val, char *child) {
  BST *n = getNode(node, val);
  if (n == NULL) {
    return NULL;
  }
  return getNode(node->children, child);
}

bool checkChild(BST *node, char *val, char *child) {
  return getChild(node, val, child) == NULL;
}

BST *removeChild(BST *node, char *val, char *child) {
  BST *n = getNode(node, val);
  if (n == NULL) {
    return NULL;
  }
  return removeNode(node->children, child);
}

bool anyInTree(BST *node, char *s, bool (*fun)(BST *, char *)) {
  if (node == NULL) {
    return false;
  }
  return fun(node->left, s) || fun(node, s) || fun(node->right, s);
}

bool anyInTree2(BST *node, char *s, char *s2, bool (*fun)(BST *, char *, char *)) {
  if (node == NULL) {
    return false;
  }
  return fun(node->left, s, s2) || fun(node, s, s2) || fun(node->right, s, s2);
}

void printTree(BST *node) {
  if (node == NULL) {
    return;
  }
  printTree(node->left);
  fprintf(stdout, "%s\n",node->value);
  printTree(node->right);
}
