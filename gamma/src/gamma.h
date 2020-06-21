/** @file
 * Interfejs klasy przechowującej stan gry gamma.
 *
 * @author Hubert Badocha <h.badocha@student.uw.edu.pl>
 * @author Marcin Peczarski <marpe@mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 15.05.2020
 */

#ifndef GAMMA_H
#define GAMMA_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Numer "gracza" oznaczający puste pole.
 */
#define EMPTY_FIELD 0

/**
 * @brief Struktura przechowująca stan gry
 */
typedef struct gamma gamma_t;

/** @brief Tworzy strukturę przechowującą stan gry.
 * Alokuje pamięć na nową strukturę przechowującą stan gry.
 * Inicjuje tę strukturę tak, aby reprezentowała początkowy stan gry.
 * @param[in] width   – szerokość planszy, liczba dodatnia,
 * @param[in] height  – wysokość planszy, liczba dodatnia,
 * @param[in] players_count – liczba graczy, liczba dodatnia,
 * @param[in] max_areas   – maksymalna liczba obszarów,
 *                      jakie może zająć jeden gracz, liczba dodatnia.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci lub któryś z parametrów jest niepoprawny.
 */
gamma_t *gamma_new(uint32_t width, uint32_t height, uint32_t players_count, uint32_t max_areas);

/** @brief Usuwa strukturę przechowującą stan gry.
 * Usuwa z pamięci strukturę wskazywaną przez @p g.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param[in,out] g   – wskaźnik na usuwaną strukturę.
 */
void gamma_delete(gamma_t *g);

/** @brief Wykonuje ruch.
 * Ustawia pionek gracza @p player na polu (@p x, @p y).
 * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia nie większa od wartości
 *                      @p players z funkcji @ref gamma_new,
 * @param[in] x       – numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       – numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 * @return Wartość @p true, jeśli ruch został wykonany, a @p false,
 * gdy ruch jest nielegalny lub któryś z parametrów jest niepoprawny.
 */
bool gamma_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);

/** @brief Wykonuje złoty ruch.
 * Ustawia pionek gracza @p player na polu (@p x, @p y) zajętym przez innego
 * gracza, usuwając pionek innego gracza.
 * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia nie większa od wartości
 *                      @p players z funkcji @ref gamma_new,
 * @param[in] x       – numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       – numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 * @return Wartość @p true, jeśli ruch został wykonany, a @p false,
 * gdy gracz wykorzystał już swój złoty ruch, ruch jest nielegalny
 * lub któryś z parametrów jest niepoprawny.
 */
bool gamma_golden_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);

/** @brief Podaje liczbę pól zajętych przez gracza.
 * Podaje liczbę pól zajętych przez gracza @p player.
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia nie większa od wartości
 *                      @p players z funkcji @ref gamma_new.
 * @return Liczba pól zajętych przez gracza lub zero,
 * jeśli któryś z parametrów jest niepoprawny.
 */
uint64_t gamma_busy_fields(const gamma_t *g, uint32_t player);

/** @brief Podaje liczbę pól, jakie jeszcze gracz może zająć.
 * Podaje liczbę wolnych pól, na których w danym stanie gry gracz @p player może
 * postawić swój pionek w następnym ruchu.
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia nie większa od wartości
 *                      @p players z funkcji @ref gamma_new.
 * @return Liczba pól, jakie jeszcze może zająć gracz lub zero,
 * jeśli któryś z parametrów jest niepoprawny.
 */
uint64_t gamma_free_fields(const gamma_t *g, uint32_t player);

/** @brief Sprawdza, czy gracz może wykonać złoty ruch.
 * Sprawdza, czy gracz @p player jeszcze nie wykonał w tej rozgrywce złotego
 * ruchu i jest przynajmniej jedno pole zajęte przez innego gracza,
 * które on może zająć bez rozbicia pola poprzedniego gracza
 * na więcej niż maksymalną liczbę obszarów oraz czy zajęcie tego pola
 * nie zwiększy liczby obszarów gracza ponad limit.
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia nie większa od wartości
 *                      @p players z funkcji @ref gamma_new.
 * @return Wartość @p true, jeśli gracz jeszcze nie wykonał w tej rozgrywce
 * złotego ruchu i jest przynajmniej jedno pole zajęte przez innego gracza,
 * a @p false w przeciwnym przypadku.
 */
bool gamma_golden_possible(gamma_t *g, uint32_t player);

/** @brief Daje napis opisujący stan planszy.
 * Alokuje w pamięci bufor, w którym umieszcza napis zawierający tekstowy
 * opis aktualnego stanu planszy. Przykład znajduje się w pliku gamma_test.c.
 * Funkcja wywołująca musi zwolnić ten bufor.
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry.
 * @return Wskaźnik na zaalokowany bufor zawierający napis opisujący stan
 * planszy lub NULL, jeśli nie udało się zaalokować pamięci.
 */
char *gamma_board(const gamma_t *g);

/**
 * @brief Zwraca z ilu znaków ma, się składać polę w gamma_board.
 * Uzależniona jest ona od liczby cyfr gracza o największym numerze, który ma pionek na planszy.
 * Jeżeli plansza jest pusta zwraca 1.
 * @param[in]         -g wskaźnik na strukturę trzymającą stan gry
 * @return Liczba znaków, z której ma się składać dane pole.
 */
int gamma_board_cell_size(const gamma_t *g);

/**
 * @brief Funkcja zwracająca szerokość planszy.
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry.
 * @return Szerokość planszy.
 */
uint32_t gamma_width(const gamma_t *g);

/**
 * @brief Funkcja zwracająca wysokość planszy.
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry.
 * @return Wysokość planszy.
 */
uint32_t gamma_height(const gamma_t *g);

/**
 * @brief Funkcja zwracająca maksymalną liczbę obszarów jaką może mieć gracz.
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry.
 * @return Maksymalna liczba obszarów jaką może mieć gracz.
 */
uint32_t gamma_max_areas(const gamma_t *g);

/**
 * @brief Funkcja zwracająca liczbę graczy.
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry.
 * @return Liczba graczy.
 */
uint32_t gamma_players(const gamma_t *g);

/**
 * @brief Funkcja zwracająca czy dany gracz nie wykorzystał jeszcze złotego ruchu.
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry.
 * @param[in] player  - numer gracza.
 * @return @p true jeżeli gracz nie wykorzystał złotego ruchu, @p false w przeciwnym przypadku.
 */
bool gamma_has_golden(const gamma_t *g, uint32_t player);

/**
 * @brief Zwraca gracza na danym polu.
 * Nie sprawdza poprawności podanych argumentów.
 * @param g           - wskaźnik na strukturę przechowującą stan gry.
 * @param x           - pierwsza współrzędna.
 * @param y           - druga współrzędna.
 * @return @p 0 jeżeli podane pole jest puste, w przeciwnym przypadku numer gracza.
 */
uint32_t gamma_player(const gamma_t *g, uint32_t x, uint32_t y);

/**
 * @brief Zwraca liczbę obszarów zajętych przez danego gracza.
 * Nie sprawdza poprawności podanych argumentów.
 * @param g           - wskaźnik na strukturę przechowującą stan gry.
 * @param player      - numer gracza.
 * @return liczba obszarów zajętych przez danego gracza.
 */
uint32_t gamma_player_areas(const gamma_t *g, uint32_t player);

#endif /* GAMMA_H */
