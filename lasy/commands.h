/**
 * Moduł obsługujący komendy
 */

//
// Created by badochov on 01.03.2020.
//

#ifndef LASY__COMMANDS_H_
#define LASY__COMMANDS_H_

#include "BST.h"
#include "input.h"

/**
 * Funkcja dodająca wezeł do drzewa na dane zagłebienie
 *
 * Jeżeli węzeł o szukanym kluczu nie bedzie istniał to zostanie stworzony
 *
 * @param node korzeń drzewa
 * @param keys tablica kluczy drzew na kolejnych zagłebieniach, ostatni klucz to klucz węzła który bedzie dodany
 * @param keysSize rozmiar tablicy keys
 * @return wskażnik na drzewo po dodaniu węzła
 */
BST add(BST node, char **keys, size_t keysSize);

/**
 * Funkcja usuwający wezeł w drzewie na danym zagłebienieniiu
 *
 * Jeżeli węzeł o szukanym kluczu nie bedzie istniał na jakiś zagłebieniu funckja nie modyfkucje drzewa
 *
 * @param node korzeń drzewa
 * @param keys tablica kluczy drzew na kolejnych zagłebieniach
 * @param keysSize rozmiar tablicy keys
 * @return wskażnik na drzewo po usunieciu węzła
 */
BST del(BST node, char **keys, size_t keysSize);

/**
 * Funkcja sprawdzający czy wezeł o danym kluczu istenieje w drzewie na danym zagłebienieniiu
 *
 * "*" w argumencie na danym zagłebieniu oznacza szukaj we wszystkich podrzewach, "*" na ostanim argumencie nie jest poprawna komendą
 *
 * @param node korzeń drzewa
 * @param keys tablica kluczy drzew na kolejnych zagłebieniach
 * @param keysSize rozmiar tablicy keys
 * @return true jeżeli istenije wpp false
 */
bool check(BST node, char **keys, size_t keysSize);

/**
 * Funkcja wypisująca wszystkie węzły na danym zagłebieniu w kolejności alfabetycznej
 *
 * @param node korzeń drzewa
 * @param keys tablica kluczy drzew na kolejnych zagłebieniach
 * @param keysSize rozmiar tablicy keys
 * @param out plik do kótrego drzewo ma zostać wypisane
 */
void print(BST node, char **keys, size_t keysSize, FILE *out);

/**
 * Wypisuje ERROR\n do podanego pliku
 * @param f plik
 */
void logError(FILE *f);

/**
 * Sprawdza jaka komenda powinna być wykonana i ją wykonuje na podanym drzewie
 * @param line obiekt lini
 * @param node korzeń drzewa na którym ma zostać wykonana komenda
 * @param out plik gdzie mają być wypisywane wyniki poprawnych komend
 * @param err plik gdzie mają być logowane errory
 * @return drzewo po wykananiu na nim komendy
 */
BST executeCommand(Line *line, BST node, FILE *out, FILE *err);

/**
 * Waliduje i wykonuje komende CHECK z argumentami z linii na danym drzewie
 * @param line linia
 * @param node korzeń drzewa
 * @param out plik gdzie mają być wypisywane wyniki poprawnych komend
 * @param err plik gdzie mają być logowane errory
 */
void executeCheck(Line *line, BST node, FILE *out, FILE *err);
/**
 * Waliduje i wykonuje komende PRINT z argumentami z linii na danym drzewie
 * @param line linia
 * @param node korzeń drzewa
 * @param out plik gdzie mają być wypisywane wyniki poprawnych komend
 * @param err plik gdzie mają być logowane errory
 */
void executePrint(Line *line, BST node, FILE *out, FILE *err);

/**
 * Waliduje i wykonuje komende ADD z argumentami z linii na danym drzewie
 * @param line linia
 * @param node korzeń drzewa
 * @param out plik gdzie mają być wypisywane wyniki poprawnych komend
 * @param err plik gdzie mają być logowane errory
 * @return drzewo po wykonaniu komendy
 */
BST executeAdd(Line *line, BST node, FILE *out, FILE *err);

/**
 * Waliduje i wykonuje komende DEL z argumentami z linii na danym drzewie
 * @param line linia
 * @param node korzeń drzewa
 * @param out plik gdzie mają być wypisywane wyniki poprawnych komend
 * @param err plik gdzie mają być logowane errory
 * @return drzewo po wykonaniu komendy
 */
BST executeDel(Line *line, BST node, FILE *out, FILE *err);

#endif //LASY__COMMANDS_H_
