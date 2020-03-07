//
// Created by badochov on 08.03.2020.
//

#ifndef LASY__UTIL_H_
#define LASY__UTIL_H_

/**
 * Zmienia wilekość tablicy na wskazaną
 * @param arr tablica
 * @param len nowa długość
 * @return tablica po zmianie wielkości
 */
void *resize(void *arr, size_t len);


/**
 * Alokuje pamieć sprawdzajac czy operacja sie powiodła
 * @param size rozmiar pamięci potrzebny do zaalokowania
 * @return wskaźnik na zaalokowane miejsce w pamięci
 */
void *alloc(size_t size);

#endif //LASY__UTIL_H_
