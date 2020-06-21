/** @file
 * Gra gamma w trybie wsadowym i interaktywnym.
 *
 * @author Hubert Badocha <h.badocha@student.uw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 22.04.2020
 */

#include "batch_mode.h"

/** @brief Główna funkcja programu
 * @return EXIT_SUCCESS, gdy wszystko przebiegło poprawnie,
 * a w przeciwnym przypadku EXIT_FAILURE.
 */
int main() {
  return batch_mode_start();
}
