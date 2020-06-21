/** @file
 * Deklaracja obsługi trybu wsadowego i wyboru trybu.
 *
 * @author Hubert Badocha <h.badocha@student.uw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 28.04.2020
 */


#ifndef GAMMA_SRC_BATCH_MODE_H_
#define GAMMA_SRC_BATCH_MODE_H_

/**
 * @brief Przeprowadza tryb wsadowy programu włącznie z wyborem trybu.
 * @return @p EXIT_FAILURE jeżeli wystąpił błąd krytyczny, @p EXIT_SUCCESS w przeciwnym przypadku.
 */
int batch_mode_start();

#endif //GAMMA_SRC_BATCH_MODE_H_
