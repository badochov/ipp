#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "BST.h"
//
// Created by badochov on 01.03.2020.
//


BST *addNode(BST *node, t val) {
  if (node == NULL) {
    BST *node;
    node->value = val;
    return node;
  }
  if (val > node->value) {
    node->right = addNode(node->right, val);
  } else if (val < node->value) {
    node->left = addNode(node->left, val);
  }
  return node;
}

BST *getNode(BST *node, t val) {
  if (node == NULL) {
    return NULL;
  }
  if (val == node->value) {
    return node;
  } else if (val < node->value) {
    return getNode(node->left, val);
  } else {
    return getNode(node->right, val);
  }
}

bool checkNode(BST *node, t val) {
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

BST *removeNode(BST *node, t val) {
  BST *n = getNode(node, val);
  if (n == NULL) {
    return NULL;
  }
  deleteNode(n);

  return node;
}

BST *addChild(BST *node, t val, t child) {
  BST *n = getNode(node, val);
  if (n == NULL) {
    return NULL;
  }
  addNode(n->children, child);
  return node;
}

BST *getChild(BST *node, t val, t child) {
  BST *n = getNode(node, val);
  if (n == NULL) {
    return NULL;
  }
  return getNode(node->children, child);
}

bool checkChild(BST *node, t val, t child) {
  return getChild(node, val, child) == NULL;
}

BST *removeChild(BST *node, t val, t child) {
  BST *n = getNode(node, val);
  if (n == NULL) {
    return NULL;
  }
  return removeNode(node->children, child);
}

bool anyInTree(BST *node, t s, bool (*fun)(BST *, t)) {
  if (node == NULL) {
    return true;
  }
  return fun(node->left, s) || fun(node, s) || fun(node->right, s);
}

bool anyInTree2(BST *node, t s, t s2, bool (*fun)(BST *, t, t)) {
  if (node == NULL) {
    return true;
  }
  return fun(node->left, s, s2) || fun(node, s, s2) || fun(node->right, s, s2);
}

bool printHelper(BST *node, t s) {
  printf("%s\n", node->value);
  return true;
}

void printTree(BST *node) {
  anyInTree(node, "", printHelper);
}
