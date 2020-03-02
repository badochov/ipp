//
// Created by badochov on 01.03.2020.
//

#ifndef LASY__FORESTS_H_
#define LASY__FORESTS_H_

#include "BST.h"

// DODAWANIE
/**
 * jeżeli byt już istenieje procedury są ignorowane
 */

// tworzy nowy las
BST *addForest(BST *node, char *name);

// dodaje nowe drzewe w danym lesie, jeśli las nie istenieje tworzy go
BST *addTree(BST *node, char *forest, char *tree);

//dodaje nowe zwierzę na danym drzewie w danym lesie, jeżeli nie ma drzewa lub lasu tworzy je
BST *addAnimal(BST *node, char *forest, char *tree, char *animal);


// USUWANIE
/**
 * jeżeli byt nie istenije procedura jest ignorowana
 */

// usuwa las
BST *removeForest(BST *node, char *forest);

// usuwa drzewo w danym lesie
BST *removeTree(BST *node, char *forest, char *tree);

// usuwa zwierze na danym drzewie w danym lesie
BST *removeAnimal(BST *node, char *forest, char *tree, char *animal);


// SPRAWDZANIE ISTNIENIA
/**
 * metody przyjmują '*' jako nazwę bytu, gwiazdka pasuje do dowolnej nazwy
 */


// sprawdza czy istenieje las
bool checkForest(BST *node, char *forest);

// sprawdza czy istenieje dane drzewo w danym lesie
bool checkTree(BST *node, char *forest, char *tree);

// sprawdza czy istnieje dane zwierzę na danym drzewie w danym lesie
bool checkAnimal(BST *node, char *forest, char *tree, char *animal);


// WYPISYWANIE
/**
 * wypisuje nazwy bytów posortowane leksykograficznie rosnąco według rozszerzonych kodów ASCII,
 * każdą nazwę w osobnym wierszu. Jeśli nie ma żadnego bytu do wypisania, to niczego nie wypisuje.
 * */

// wypisuje wszystkie lasy
void printForests(BST *node);

// wypisuje wszystkie drzewa w danym lesie
void printTrees(BST *node, char *forest);

// wypisuje wszystkie zwierzęta na danym drzewie w danym lesie
void printAnimals(BST *node, char *forest, char *tree);

#endif //LASY__FORESTS_H_
