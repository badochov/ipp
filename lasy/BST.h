/**
 * Moduł drzew BST z dziećmi
 */
//
// Created by badochov on 01.03.2020.
//
#ifndef LASY_BST_H
#define LASY_BST_H

#include <stdbool.h>
#include <stdio.h>

/**
 * Struktura trzymająca węzeł drzewa BST
 */
struct Node {
  /** Klucz */
  char *key;
  /** Wskaźnik na lewo podrzewo */
  struct Node *left;
  /** wskaźnik na lewe podrzweo */
  struct Node *right;
  /** wskaźnik na dzieci */
  struct Node *children;
};

typedef struct Node *BST;

/**
 * Inicjalizuje nowe drzewo z podanym kluczem
 * @param key klucz
 * @return wskaźnik do zinizjalizowanego drzewa
 */
BST init(char *key);

/**
 * Dodaje węzeł o podanych kluczu do danego drzewa
 * @param node korzeń drzewa
 * @param key klucz
 * @return drzewo po dodaniu nowego węzła
 */
BST addNode(BST node, char *key);

/**
 * Szuka w danym drzewie wezłą o danym kluczu
 * @param node korzeń drzewa
 * @param key klucz
 * @return wskaźnik do szukanego węzła lub NULL jeżeli taki węzeł nie istenieje
 */
BST getNode(BST node, char *key);

/**
 * Sprawdza czy w danym drzewie istnieje węzeł o podanyn kluczu
 * @param node korzeń drzewa
 * @param key klucz
 * @return true jeżeli węzeł istnieje wpp false
 */
bool existsNode(BST node, char *key);

/**
 * Usuwa wezęł o podanym kluczu z drzewa
 * @param node korzeń drzewa
 * @param key klucz
 * @return wskaźnik na korzeń drzewa po usunięciu
 */
BST removeNode(BST node, char *key);

/**
 * Zwraca dzieci danego węzła
 * @param node węzeł
 * @return wskaźnik do drzewa dzieci
 */
BST getChildren(BST node);

/**
 * Usuwa drzewo z pamięci
 * @param node wskaźnik do korzenia drzewa
 * @return NULL
 */
BST deleteTree(BST node);

/**
 * Wykonuje daną funckje na kazdym węźle w danym drzewie w porządku infixowym
 * @param node korzeń drzewa
 * @param fun funkcja przyjmująca kolejne węzły
 */
void traverseTreeInOrder(BST node, void (*fun)(BST, FILE *), FILE *f);

#endif //LASY_BST_H
