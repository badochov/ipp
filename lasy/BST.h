//
// Created by badochov on 01.03.2020.
//
#ifndef LASY_BST_H
#define LASY_BST_H

#include <stdbool.h>

// strukura reprezentująca drzewo BST
typedef struct Node {
  char *value;
  struct Node *left;
  struct Node *right;
  struct Node *children;
} BST;

// inicjalizuje nowe drzewo z BST z podaną wartością
BST *init(char *value);

// dodaje node do danego drzewa bst
BST *addNode(BST **node, char *val);

// szuka node o podanej wartości w drzewie, zwraca wskaźnik do niego jeśli istenieje wpp null
BST *getNode(BST *node, char *val);

// sprawdza czy dana wartość występuje w drzewie
bool checkNode(BST *node, char *val);

// usuwa node o danej wartości z drzewa w przypadku kiedy node nie istnieje nic nie zmienia
BST *removeNode(BST **node, char *val);

// dodaje node do danego drzewa dzieci elementu o danej wartości jeśli dany element nie istnieje tworzy go i dodaje dziecko
BST *addChild(BST **node, char *val, char *child);

// szuka dziecka o podanej wartości w drzewie dzieci podanego elementu o danej wartości, zwraca wskaźnik do niego jeśli istenieje wpp null
BST *getChild(BST *node, char *val, char *child);

// sprawdza czy node o danej wartości ma dziecko o podanej wartości jeżelie node nie istenieje zwraca false
bool checkChild(BST *node, char *val, char *child);

// usuwa dziecko o danej wartosći z node o danej wartości, jeżeli jedno z nich nie istnieje nic nie zmienia
BST *removeChild(BST **node, char *val, char *child);

// usuwa drzewo BST
void deleteTree(BST **node);

// wypisuje drzewo w porządku infixowym
void printTree(BST *node);

#endif //LASY_BST_H
