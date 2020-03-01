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
BST *addForest(BST *node, t name);

// dodaje nowe drzewe w danym lesie, jeśli las nie istenieje tworzy go
BST *addTree(BST *node, t forest, t tree);

//dodaje nowe zwierzę na danym drzewie w danym lesie, jeżeli nie ma drzewa lub lasu tworzy je
BST *addAnimal(BST *node, t forest, t tree, t animal);


// USUWANIE
/**
 * jeżeli byt nie istenije procedura jest ignorowana
 */

// usuwa las
BST *removeForest(BST *node, t forest);

// usuwa drzewo w danym lesie
BST *removeTree(BST *node, t forest, t tree);

// usuwa zwierze na danym drzewie w danym lesie
BST *removeAnimal(BST *node, t forest, t tree, t animal);


// SPRAWDZANIE ISTNIENIA
/**
 * metody przyjmują '*' jako nazwę bytu, gwiazdka pasuje do dowolnej nazwy
 */


// sprawdza czy istenieje las
bool checkForest(BST *node, t forest);

// sprawdza czy istenieje dane drzewo w danym lesie
bool checkTree(BST *node, t forest, t tree);

// sprawdza czy istnieje dane zwierzę na danym drzewie w danym lesie
bool checkAnimal(BST *node, t forest, t tree, t animal);


// WYPISYWANIE
/**
 * wypisuje nazwy bytów posortowane leksykograficznie rosnąco według rozszerzonych kodów ASCII,
 * każdą nazwę w osobnym wierszu. Jeśli nie ma żadnego bytu do wypisania, to niczego nie wypisuje.
 * */

// wypisuje wszystkie lasy
void printForests(BST *node);

// wypisuje wszystkie drzewa w danym lesie
void printTrees(BST *node, t forest);

// wypisuje wszystkie zwierzęta na danym drzewie w danym lesie
void printAnimals(BST *node, t forest, t tree);

#endif //LASY__FORESTS_H_
