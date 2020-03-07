//
// Created by badochov on 01.03.2020.
//

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "BST.h"
#include "util.h"

#define IS_NODE_NULL(n) if(n == NULL){return NULL;}

BST init(char *value) {
  BST node = alloc(sizeof(struct Node));
  node->key = alloc(sizeof(char) * (strlen(value) + 1));
  strcpy(node->key, value);
  node->right = NULL;
  node->left = NULL;
  node->children = NULL;
  return node;
}

BST addNode(BST node, char *val) {
  if (node == NULL) {
    node = init(val);
    return node;
  }
  int cmp = strcmp(val, node->key);
  if (cmp < 0) {
    node->left = addNode(node->left, val);
  } else if (cmp > 0) {
    node->right = addNode(node->right, val);
  }
  return node;
}

BST getNode(BST node, char *key) {
  IS_NODE_NULL(node);

  int cmp = strcmp(key, node->key);
  if (cmp == 0) {
    return node;
  } else if (cmp < 0) {
    return getNode(node->left, key);
  } else {
    return getNode(node->right, key);
  }
}

bool existsNode(BST node, char *val) {
  return getNode(node, val) != NULL;
}

BST deleteTree(BST node) {
  IS_NODE_NULL(node);

  free(node->key);
  deleteTree(node->left);
  deleteTree(node->children);
  deleteTree(node->right);
  free(node);

  return NULL;
}

/**
 * Zwraca wskaźnik do węzła w drzewie o najmniejszym kluczy
 * @param node korzeń drzewa którego szukamy
 * @return wskaźnik do węzła o najmniejszym kluczu w drzewie
 */
BST smallestNode(BST node) {
  while (node && node->left) {
    node = node->left;
  }
  return node;
}

/**
 * Funckja pomocnicza usuwająca węzeł o podanym kluczu
 * @param node korzeń drzewa w którym szukamy
 * @param key klucz
 * @param freeMem parametr mówiący czy zwolnić pamięć po dzieciach i kluczu szukanego węzła
 * @return
 */
BST rNode(BST node, char *key, bool freeMem) {
  IS_NODE_NULL(node);

  int cmp = strcmp(node->key, key);
  if (cmp == 0) {
    if (freeMem) {
      node->children = deleteTree(node->children);
      free(node->key);
    }

    if (node->right == NULL) {
      BST temp = node->left;
      free(node);
      return temp;
    }
    BST n = smallestNode(node->right);
    node->key = n->key;
    node->children = n->children;
    node->right = rNode(node->right, n->key, false);
  } else if (cmp > 0) {
    node->left = rNode(node->left, key, freeMem);
  } else {
    node->right = rNode(node->right, key, freeMem);
  }
  return node;
}

BST removeNode(BST node, char *key) {
  return rNode(node, key, true);
}

void traverseTreeInOrder(BST node, void (*fun)(BST, FILE *), FILE *f) {
  if (node == NULL) {
    return;
  }
  traverseTreeInOrder(node->left, fun, f);
  fun(node, f);
  traverseTreeInOrder(node->right, fun, f);
}

BST getChildren(BST node) {
  IS_NODE_NULL(node);

  return node->children;
}
