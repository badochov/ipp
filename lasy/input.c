//
// Created by badochov on 02.03.2020.
//

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "util.h"

#define CHECK_EOF(c, f) if (c == EOF && feof(f)) { return 2; }


/**
 * Wczytuje stringa (ASCII 33-255) ignorujać białe znaki go pporzedzajace
 * @param s referencja do miejsca w którym string ma być zapisany
 * @param f plik z którego string ma być czytany
 * @return -1 -> nielegealny znak zostal wczytany
 *          0 -> nie ma nic do wczytania
 *          1 -> wczytanie przebiegło pomyślnie
 *          2 -> koniec pliku został osiagniety
 */
short readString(char **s, FILE *f) {
  int size = 0;
  int n = 0;
  char c = getc(f);
  while (c != '\n') {
    CHECK_EOF(c, f);
    if (!isspace(c)) {
      if (c < 33) {
        return -1;
      }
      if (n == 0) {
        *s = alloc(sizeof(char));
        size = 1;
      }
      (*s)[n++] = c;
      if (n == size) {
        size *= 2;
        *s = resize(*s, size);
      }
    } else if (n > 0) {
      break;
    }
    c = getc(f);
  }
  if (size) {
    (*s)[n++] = '\0';
    *s = resize(*s, n);
  }

  if (c == '\n') {
    ungetc(c, f);
  }
  return (short) (n > 1);
}

/**
 * SPrawdza czy są argumenty dodatkowe do komendy
 * @param f plik z ktorego argumenty mają być czytane
 * @return kody jak w readString
 */
short readExtra(FILE *f) {
  char c = getc(f);
  while (c != '\n') {
    CHECK_EOF(c, f);
    if (!isspace(c)) {
      if (c < 33) {
        return -1;
      }
      return 1;
    }
    c = getc(f);
  }
  ungetc(c, f);
  return 0;
}

/**
 * Wczyuję komende lub komentarz
 * @param s referencja do zmiennej w której ma być zapisana komenda
 * @param f plik z którego czyta komenda
 * @return 0 -> komentarz oraz takie same kody jak readString
 */
short readCommand(char **s, FILE *f) {
  char c = getc(f);
  if (c == '#') {
    *s = alloc(sizeof(char) * 2);
    (*s)[0] = '#';
    (*s)[1] = '\0';
    return 0;
  }
  CHECK_EOF(c, f);
  ungetc(c, f);
  return readString(s, f);
}

/**
 * Dochodzi do końca linii
 * @param f plik
 * @return jeżeli plik zakończył się przed kończem lini false true wpp
 */
bool reachEOL(FILE *f) {
  while (getc(f) != '\n') {
    if (feof(f)) {
      return false;
    }
  }
  return true;
}

Line *initLine(size_t argsCount) {
  Line *l;
  l = alloc(sizeof(Line));
  l->args = alloc(sizeof(char *) * argsCount);
  for (size_t i = 0; i < argsCount; i++) {
    l->args[i] = NULL;
  }
  l->size = 0;
  l->maxSize = argsCount;
  l->eof = l->ill = false;
  return l;
}

Line *readLine(FILE *f, size_t argsCount) {
  Line *res = initLine(argsCount);
  short readOutcome = 0;
  for (size_t i = 0; i < argsCount + 1; i++) {
    if (i == 0) {
      readOutcome = readCommand(&res->args[i], f);
    } else if (i == argsCount) {
      readOutcome = readExtra(f);
    } else {
      readOutcome = readString(&res->args[i], f);
    }
    if (readOutcome == -1 || readOutcome == 0 || readOutcome == 2) {
      break;
    }
    res->size++;
  }
  if (readOutcome == -1) {
    res->ill = true;
  }
  if (readOutcome == 2) {
    res->eof = true;
  } else {
    res->eof = !reachEOL(f);
  }

  return res;
}

void freeLine(Line *line) {
  if (line == NULL) {
    return;
  }
  for (size_t i = 0; i < line->maxSize; i++) {
    free(line->args[i]);
  }
  free(line->args);
  free(line);
}
