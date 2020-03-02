//
// Created by badochov on 01.03.2020.
//

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "forests.h"

// szuka lasu o danej nazwie, jeżeli go nie ma zwraca NULL
BST *getForest(BST *node, char *forest) {
  return getNode(node, forest);
}

// szuka lasu o drzewa nazwie w danym lesie, jeżeli las lub drzewo nie występuje zwraca NULL
BST *getTree(BST *node, char *forest, char *tree) {
  return getChild(node, forest, tree);
}

// szuka podanego zwierzęcia w danym lesie na danym drzewie, jeżeli coś z tej trójki nie występuje zwraca NULL
BST *getAnimal(BST *node, char *forest, char *tree, char *animal) {
  BST *tr = getTree(node, forest, tree);
  return getNode(tr, animal);
}

BST *addForest(BST *node, char *name) {
  return addNode(node, name);
}

BST *addTree(BST *node, char *forest, char *tree) {
  return addChild(node, forest, tree);
}

BST *addAnimal(BST *node, char *forest, char *tree, char *animal) {
  BST *tr = getTree(node, forest, tree);
  return addNode(tr, animal);
}

BST *removeForest(BST *node, char *forest) {
  return removeNode(node, forest);
}

BST *removeTree(BST *node, char *forest, char *tree) {
  return removeChild(node, forest, tree);
}

BST *removeAnimal(BST *node, char *forest, char *tree, char *animal) {
  BST *tr = getTree(node, forest, tree);
  return removeNode(tr, animal);
}


bool checkForest(BST *node, char *forest) {
  printf("siema");
  if (forest == NULL) {
    return false;
  }
  return getForest(node, forest) == NULL;
}

bool checkTree(BST *node, char *forest, char *tree) {
  if (forest == NULL || tree == NULL) {
    return false;
  }
  if (strcmp(forest, "*") == 0) {
    return anyInTree(node, tree, checkForest);
  }
  return checkForest(getForest(node, forest), tree);
}

// funckja pomocnicza przy szukaniu zwierzęcia, przeszukuje cały las w poszukinaiu zwierzęcia na danym drzewie
bool checkAnimalHelper(BST *node, char *tree, char *animal) {
  if (node == NULL) {
    return false;
  }
  return checkAnimalHelper(node->left, tree, animal)
      || checkTree(node, tree, animal)
      || checkAnimalHelper(node->right, tree, animal);
}

bool checkAnimal(BST *node, char *forest, char *tree, char *animal) {
  if (forest == NULL || tree == NULL || animal == NULL) {
    return false;
  }
  if (strcmp(forest, "*") == 0) {
    return anyInTree2(node, forest, animal, checkAnimalHelper);
  }
  BST *f = getForest(node, forest);
  if (f == NULL) {
    return false;
  }
  return checkTree(f->children, tree, animal);
}

void printForests(BST *node) {
  printTree(node);
}

void printTrees(BST *node, char *forest) {
  BST *f = getForest(node, forest);
  if (f == NULL) {
    return;
  }
  printTree(f->children);
}

void printAnimals(BST *node, char *forest, char *tree) {
  BST *tr = getTree(node, forest, tree);
  if (tr == NULL) {
    return;
  }
  printTree(tr->children);
}