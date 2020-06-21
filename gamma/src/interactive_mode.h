/** @file
 * Deklaracja obsługi trybu interpretowanego.
 *
 * @author Hubert Badocha <h.badocha@student.uw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 28.04.2020
 */

#ifndef GAMMA_SRC_INTERACTIVE_MODE_H_
#define GAMMA_SRC_INTERACTIVE_MODE_H_

#include "gamma.h"

/**
 * @brief Przeprowadza tryb interaktywny programu.
 * @param[in,out] g wskaźnik na strukturę przechowującą stan gry.
 * @return @p EXIT_FAILURE jeżeli wystąpił błąd krytyczny, @p EXIT_SUCCESS w przeciwnym przypadku.
 */
int interactive_mode_start(gamma_t *g);

#endif //GAMMA_SRC_INTERACTIVE_MODE_H_
