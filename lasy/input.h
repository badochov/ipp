//
// Created by badochov on 02.03.2020.
//

#ifndef LASY__INPUT_H_
#define LASY__INPUT_H_

#include <stdbool.h>
#include <stdio.h>

typedef struct {
  /** Czy EOF został osiągniety */
  bool eof;
  /** Czy byly jakiś nielegalne znaki (ASCII < 33) */
  bool ill;
  /** Wczytane argumenty */
  char **args;
  /** Liczba wczytanyvh argumentów*/
  size_t size;
  /** Maksymalna liczba argumentów do wczytania */
  size_t maxSize;
} Line;

/**
 * Inicjalizuje nową linie o podanej liczbie arguemntów
 * @param argsCount liczba argumentów
 * @return linia
 */
Line *initLine(size_t argsCount);

/**
 * Wczytuje linie z pliku
 * @param f plik z którego linia będzie wczytana
 * @param argsSize maksymalna liczba arguemntów
 * @return struktura odpowiedzi
 */
Line *readLine(FILE *f, size_t argsSize);

/**
 * Zwalnia pamięć zajmowaną przez linię
 * @param line linia
 */
void freeLine(Line *line);

#endif //LASY__INPUT_H_
