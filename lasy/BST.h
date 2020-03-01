//
// Created by badochov on 01.03.2020.
//
#ifndef LASY_BST_H
#define LASY_BST_H

#include <stdbool.h>

#define t char*

// strukura reprezentująca drzewo BST
typedef struct Node {
  t value;
  struct Node *left;
  struct Node *right;
  struct Node *children;
} BST;

// dodaje node do danego drzewa bst
BST *addNode(BST *node, t val);

// szuka node o podanej wartości w drzewie, zwraca wskaźnik do niego jeśli istenieje wpp null
BST *getNode(BST *node, t val);

// sprawdza czy dana wartość występuje w drzewie
bool checkNode(BST *node, t val);

// usuwa node o danej wartości z drzewa w przypadku kiedy node nie istnieje nic nie zmienia
BST *removeNode(BST *node, t val);

// dodaje node do danego drzewa dzieci elementu o danej wartości jeśli dany element nie istnieje tworzy go i dodaje dziecko
BST *addChild(BST *node, t val, t child);

// szuka dziecka o podanej wartości w drzewie dzieci podanego elementu o danej wartości, zwraca wskaźnik do niego jeśli istenieje wpp null
BST *getChild(BST *node, t val, t child);

// sprawdza czy node o danej wartości ma dziecko o podanej wartości jeżelie node nie istenieje zwraca false
bool checkChild(BST *node, t val, t child);

// usuwa dziecko o danej wartosći z node o danej wartości, jeżeli jedno z nich nie istnieje nic nie zmienia
BST *removeChild(BST *node, t val, t child);

// usuwa drzewo BST, zwraca true w wypadku powodzenia, false w przyapdku błedu
bool deleteTree(BST *node);

// wywołuje funkcje na każdym elemenecie drzewa w porządku infixowym i sprawdza czy jakikolwiek element spełnia warunek
bool anyInTree(BST *node, t s, bool (*fun)(BST *, t));

bool anyInTree2(BST *node, t s, t s2, bool (*fun)(BST *, t, t));

// wypisuje drzewo w porządku infixowym
void printTree(BST *node);

#endif //LASY_BST_H
