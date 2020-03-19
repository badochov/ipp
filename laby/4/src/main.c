/** @file
 * Główny modułu generatora liczb pseudolosowych
 *
 * @author Igrek Iksiński <yx@mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 09.03.2020
 */

#include "input-output.h"
#include "generator.h"

/** @brief Generuje zadaną ilość liczb pseudolosowych.
 * Czyta liczbę @p count i wypisuje @p count liczb pseudolosowych.
 * @return 0 – jeśli program zakończył się sukcesem.
 */
int main(void) {
  int count;
  int i;

  count = readInt();

  for (i = 0; i < count; ++i) {
    printUint(generateRandomUint());
  }

  return 0;
}
