//
// Created by badochov on 07.03.2020.
//

#ifndef LASY__VALIDATOR_H_
#define LASY__VALIDATOR_H_

#include <stdbool.h>
#include "input.h"
typedef struct {
  /** Czy program powinien przejśc do następnego obrotu */
  bool c;
  /**  Czy program powinine zostać przerwany */
  bool b;
  /** Czy error powinien zostać wypisany */
  bool err;
} Validator;

/**
 * Inicjalizuje walidator ze wszystkimi atrybutami ustawionymi na false;
 * @return walidator
 */
Validator *initValidator();

/**
 * Waliduje linie
 * @param line linia która będzie walidowana
 * @return obiekt Validatora
 */
Validator *validateLine(Line *line);

/**
 * Waliduje argumenty do komendy add
 * @param args
 * @param maxArgs
 * @return
 */

/**
 * Waliduje komende ADD
 * @param argCount liczba przezywanych argumentów
 * @param maxArgs maksymalna liczba argumentów dla tej komendy
 * @return true jeżeli dane są poprawne wpp false
 */
bool validateAdd(size_t argCount, size_t maxArgs);

/**
 * Waliduje komende DEL
 * @param argCount liczba przezywanych argumentów
 * @param maxArgs maksymalna liczba argumentów dla tej komendy
 * @return true jeżeli dane są poprawne wpp false
 */
bool validateDel(size_t argCount, size_t maxArgs);

/**
 * Waliduje komende PRINT
 * @param argCount liczba przezywanych argumentów
 * @param maxArgs maksymalna liczba argumentów dla tej komendy
 * @return true jeżeli dane są poprawne wpp false
 */
bool validatePrint(size_t argCount, size_t maxArgs);

/**
 * Waliduje komende CHECK
 * @param args argumenty przezaywane potem do komendy add
 * @param argCount liczba przezywanych argumentów
 * @param maxArgs maksymalna liczba argumentów dla tej komendy
 * @return true jeżeli dane są poprawne wpp false
 */
bool validateCheck(char **args, size_t argCount, size_t maxArgs);

/**
 * Zwalnia pamięć zajmowaną przez Walidator
 * @param v walidator
 */
void freeValidator(Validator* v);

#endif //LASY__VALIDATOR_H_
