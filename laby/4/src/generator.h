/** @file
 * Interfejs modułu generatora liczb pseudolosowych
 *
 * @author Igrek Iksiński <yx@mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 09.03.2020
 */

#ifndef GENERATOR_H
#define GENERATOR_H

/** @brief Ustawia wewnętrzny stan generatora.
 * Ustawia @p newState jako wartość, która zostanie użyta jako wewnętrzny stan
 * genratora przy następnym wywołaniu funkcji @ref generateRandomUint.
 * @param[in] newState – nowy wartość stanu.
 */
extern void setState(unsigned newState);

/** @brief Generuje liczbę pseudolosową.
 * Generuje liczbę pseudolosową na podstawie wewnętrznego stanu generatora
 * (poprzednio wygenerowanej wartości) za pomocą mnożenia i dodawania modulo
 * potęga dwójki.
 * @return Wygenerowana liczba.
 */
extern unsigned generateRandomUint(void);

#endif /* GENERATOR_H */
