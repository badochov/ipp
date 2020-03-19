/** @file
 * Deklaracja interfejsu użytkownika
 *
 * @author Igrek Iksiński <yx@mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 09.03.2020
 */

#ifndef INPUT_OUTPUT_H
#define INPUT_OUTPUT_H

/** @brief Czyta liczbę.
 * Czyta liczbę ze znakiem ze standardowego wejścia.
 * @return Wczytana liczba.
 */
extern int readInt(void);

/** @brief Wypisuje liczbę.
 * Wypisuje liczbę bez znaku na standardowe wyjście.
 * @param[in] value – liczba do wypisania.
 */
extern void printUint(unsigned int value);

#endif /* INPUT_OUTPUT_H */
