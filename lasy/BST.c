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
  node->value = malloc(sizeof(char) * (strlen(value) + 1));
  strcpy(node->value, value);
  node->right = NULL;
  node->left = NULL;
  node->children = NULL;
  return node;
}

BST *addNode(BST **node, char *val) {
  if (*node == NULL) {
    *node = init(val);
    return *node;
  }
  int cmp = strcmp(val, (*node)->value);
  if (cmp > 0) {
    addNode(&(*node)->right, val);
  } else if (cmp < 0) {
    addNode(&(*node)->left, val);
  }
  return *node;
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
  return getNode(node, val) != NULL;
}

void deleteTree(BST **node) {
  if (node == NULL || *node == NULL) {
    return;
  }

  free((*node)->value);
  deleteTree(&(*node)->left);
  deleteTree(&(*node)->children);
  deleteTree(&(*node)->right);
  free(*node);
  *node = NULL;
}

// zwraca node o najmniejszym value z danego drzewa
BST *smallestNode(BST *node) {
  BST *c = node;
  while (c && c->left) {
    c = c->left;
  }
  return c;
}

//usuwanie node bez usuwania jego dzieci i wartosci
BST *rNode(BST *node, char *val);

// procedura pomocnicza usuwająca node'a
BST *deleteNode(BST *node) {
  if (node == NULL) {
    return NULL;
  }
  if (node->left == NULL) {
    BST *temp = node->right;
    free(node);
    return temp;
  }
  if (node->right == NULL) {
    BST *temp = node->left;
    free(node);
    return temp;
  }
  BST *n = smallestNode(node->right);
  (node)->value = n->value;
  (node)->children = n->children;\
  (node)->right = rNode(node->right, n->value);
  return node;
}

BST *rNode(BST *node, char *val) {
  if (node == NULL) {
    return NULL;
  }
  int cmp = strcmp((node)->value, val);
  if (cmp == 0) {
    node = deleteNode(node);
    return node;
  } else if (cmp > 0) {
    node->left = rNode((node)->left, val);
    return node;
  }
  node->right = rNode((node)->right, val);
  return node;
}

BST *removeNode(BST **node, char *val) {
  if (node == NULL || *node == NULL) {
    return NULL;
  }
  int cmp = strcmp((*node)->value, val);
  if (cmp == 0) {
    deleteTree(&((*node)->children));
    free((*node)->value);
    *node = deleteNode(*node);
//    printf("\t%s", (*node)->value);
    return *node;
  } else if (cmp > 0) {
    return removeNode(&(*node)->left, val);
  }
  return removeNode(&(*node)->right, val);
}

BST *addChild(BST **node, char *val, char *child) {
  addNode(node, val);
  BST *n = getNode(*node, val);
  addNode(&(n->children), child);
  return *node;
}

BST *getChild(BST *node, char *val, char *child) {
  BST *n = getNode(node, val);
  if (n == NULL) {
    return NULL;
  }
  return getNode(n->children, child);
}

bool checkChild(BST *node, char *val, char *child) {
  return getChild(node, val, child) != NULL;
}

BST *removeChild(BST **node, char *val, char *child) {
  BST *n = getNode(*node, val);
  if (n == NULL) {
    return NULL;
  }
  return removeNode(&(n->children), child);
}

void printTree(BST *node) {
  if (node == NULL) {
    return;
  }
  printTree(node->left);
  fprintf(stdout, "%s\n", node->value);
  printTree(node->right);
}
