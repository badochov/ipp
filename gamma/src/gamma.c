/** @file
 * Implementacja klasy przechowującej stan gry gamma.
 *
 * @author Hubert Badocha <h.badocha@student.uw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 8.04.2020
 */

#if defined(__STDC_LIB_EXT1__)
/** @brief Flaga ustawiająca, że chcemy sprintf_s. */
#define __STDC_WANT_LIB_EXT1__ 1
#else
/** @brief Jeżeli @p sprintf_s nie jest dostępny zastępuję do @p sprintf.*/
#define sprintf_s sprintf
#endif

#include "gamma.h"
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

/** @brief Maksymalna liczba sąsiadów komórki. */
#define NEIGHBOURS_COUNT 4

/**
 * @brief Struktura drzewa Union-Find, przechowująca też numer gracza na danym polu
 */
typedef struct union_find {
  struct union_find *up; ///< wskaźnik na rodzica, NULL znaczy że jest korzeniem

  uint32_t player; ///< gracz okupujący pole
  uint64_t rank;   ///< ranga
} union_find_t;

/**
 * @brief Struktura przechowująca dane o graczu
 */
typedef struct {
  bool used_golden;     ///< wartość sprawdzająca czy gracz wykorzystał już złoty ruch
  uint64_t busy_fields; ///< wartość trzymająca ile zajętych pól ma gracz
  uint64_t free_fields; ///< wartość trzymająca ile wolnych pól ma gracz,
  ///< jeżeli wykorzystał już wszystkie obszary
  uint64_t areas; ///< wartość trzymająca ile obszarów zajmuje gracz
} player;

/**
 * @brief Struktura przechowująca stan gry
 */
struct gamma {
  uint32_t players_count; ///< liczba graczy
  uint32_t max_areas;     ///< maksymalna liczba obszarów na gracza
  uint32_t width;         ///< szerokość planszy
  uint32_t height;        ///< wysokość planszy

  uint64_t occupied;    ///< liczba zajętych pól na planszy
  uint64_t total_areas; ///< liczba zajętych obszarów na planszy

  union_find_t **board; ///< plansza w formie drzew Union-Find

  player *players; ///< tablica z danymi o graczach
};

/**
 * @brief Stała pomocnicza do przechodzenia po sąsiadach pola.
 */
static const int NEIGHBORS[NEIGHBOURS_COUNT][2] = {
    {1, 0},
    {0, 1},
    {-1, 0},
    {0, -1},
};

/**
 * @brief Sprawdza, czy dane koordynaty są poprawne dla podanej planszy.
 * @param[in] g wskaźnik na strukturę przechowująca stan gry
 * @param[in] x pierwsza współrzędna
 * @param[in] y druga współrzędna
 * @return Wartość @p true jeżeli są poprawne, @p false w przeciwnym przypadku.
 */
static inline bool valid_coords(const gamma_t *g, int64_t x, int64_t y) {
  return x >= 0 && x < g->width && y >= 0 && y < g->height;
}

/**
 * @brief Liczy sąsiadów danego pola, których okupuje podany gracz.
 * @param[in] g wskaźnik na strukturę przechowującą stan gry
 * @param[in] player numer gracza
 * @param[in] x pierwsza współrzędna
 * @param[in] y druga współrzędna
 * @return Liczba sąsiadujących pól, które okupuje podany gracz.
 */
static inline unsigned int neighboring_player_count(const gamma_t *g,
                                                    uint32_t player,
                                                    uint32_t x,
                                                    uint32_t y) {
  unsigned int count = 0;

  for (int i = 0; i < NEIGHBOURS_COUNT; i++) {
    int64_t nx = (int64_t) x + NEIGHBORS[i][0];
    int64_t ny = (int64_t) y + NEIGHBORS[i][1];
    if (valid_coords(g, nx, ny)) {
      if (g->board[nx][ny].player == player) {
        count++;
      }
    }
  }

  return count;
}

/**
 * @brief Sprawdza, czy wstawienie na dane pole pionka danego gracza
 * nie zwiększy liczby jego obszarów ponad limit.
 * @param[in] g wskaźnik na strukturę przechowującą stan gry
 * @param[in] player numer gracza
 * @param[in] x pierwsza współrzędna
 * @param[in] y druga współrzędna
 * @return Wartość @p true jeżeli liczba obszarów nie przekroczy maksymalnej,
 * @p false w przeciwnym przypadku.
 */
static inline bool check_post_move_area_count(const gamma_t *g,
                                              uint32_t player,
                                              uint32_t x,
                                              uint32_t y) {
  if (g->players[player].areas < g->max_areas) {
    return true;
  }
  return neighboring_player_count(g, player, x, y) > 0;
}

/**
 * @brief Sprawdza, czy podany wskaźnik na strukturę przechowującą stan gry to nie @p NULL
 * @param[in] g wskaźnik na strukturę przechowującą stan gry
 * @return @p true jeżeli podany wskaźnik to nie @p NULL, w przeciwnym wypadku @p false
 */
static inline bool validate_board(const gamma_t *g) {
  return g != NULL;
}

/**
 * @brief Sprawdza, czy podany gracz jest poprawny dla danej rozgrywki.
 * Zakłada że wskaźnik na strukturę jest zwalidowany.
 * @param[in] g wskaźnik na strukturę przechowującą stan gry
 * @param[in] player numer gracza
 * @return @p true jeżeli podany gracz uczestniczy w danej rozgrywce, w przeciwnym wypadku @p false
 */
static inline bool validate_player(const gamma_t *g, uint32_t player) {
  return player != EMPTY_FIELD && g->players_count >= player;
}

/**
 * @brief Sprawdza, czy podane koordynaty znajdują się na podanej planszy.
 * Zakłada że wskaźnik na strukturę jest zwalidowany.
 * @param[in] g wskaźnik na strukturę przechowującą stan gry
 * @param[in] x pierwsza współrzędna
 * @param[in] y druga współrzędna
 * @return @p true jeżeli podane koordynaty są na danej planszy, w przeciwnym wypadku @p false
 */
static inline bool validate_coords(const gamma_t *g, uint32_t x, uint32_t y) {
  return g->width > x && g->height > y;
}

/**
 * @brief Wykonuje serie sprawdzeń czy ruch może zostać wykonany,
 * jeżeli na danym polu nie byłoby pionka.
 * Zakłada że wskaźnik na strukturę jest zwalidowany.
 * @param[in] g wskaźnik na strukturę przechowującą stan gry
 * @param[in] player numer gracza
 * @param[in] x pierwsza współrzędna
 * @param[in] y druga współrzędna
 * @return @p true jeżeli podane ruch może być wykonany jeżeli dane pole jest puste,
 * w przeciwnym wypadku @p false
 */
static inline bool validate_move(const gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
  return validate_player(g, player) && validate_coords(g, x, y)
      && check_post_move_area_count(g, player, x, y);
}

/**
 * @brief Ustawia podane pole jako zajęte przez danego gracza.
 * Aktualizuję liczbę zajętych pól przez obecnego i poprzedniego gracza.
 * Aktualizuję łączną liczbę pól zajętych w grze.
 * @param[in, out]  g wskaźnik na strukturę trzymająca stan gry.
 * @param[in] player numer gracza lub zero jeżeli pola ma zostać zwolnione.
 * @param[in] x pierwsza współrzędna.
 * @param[in] y druga współrzędna.
 */
static inline void gamma_set_player(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
  uint32_t prev_player = g->board[x][y].player;
  g->board[x][y].player = player;

  g->players[player].busy_fields++;
  g->players[prev_player].busy_fields--;

  if (player == EMPTY_FIELD) {
    g->occupied--;
  } else {
    g->occupied++;
  }
}

gamma_t *gamma_new(uint32_t width,
                   uint32_t height,
                   uint32_t players_count,
                   uint32_t max_areas) {
  if (width == 0 || height == 0 || players_count == 0 || max_areas == 0) {
    return NULL;
  }
  gamma_t *g = calloc(1, sizeof(gamma_t));
  if (g == NULL) {
    gamma_delete(g);
    return NULL;
  }

  g->players_count = players_count;
  g->width = width;
  g->height = height;
  g->max_areas = max_areas;

  g->board = calloc(width, sizeof(union_find_t *));
  if (g->board == NULL) {
    gamma_delete(g);
    return NULL;
  }

  for (uint32_t x = 0; x < width; x++) {
    g->board[x] = calloc(height, sizeof(union_find_t));
    if (g->board[x] == NULL) {
      gamma_delete(g);
      return NULL;
    }
  }

  g->players = calloc((uint64_t) players_count + 1, sizeof(player));

  if (g->players == NULL) {
    gamma_delete(g);
    return NULL;
  }

  return g;
}

void gamma_delete(gamma_t *g) {
  if (!validate_board(g)) {
    return;
  }

  free(g->players);

  if (g->board != NULL) {
    for (uint32_t x = 0; x < g->width; x++) {
      free(g->board[x]);
    }
    free(g->board);
  }

  free(g);
}

/**
 * @brief Zwraca korzeń danego drzewa Union-Find i wykonuje skracanie ścieżek.
 * @param[in,out] uf wskaźnik na drzewo union find.
 * @return Wskaźnik na korzeń drzewa.
 */
static union_find_t *root(union_find_t *uf) {
  if (uf->up == NULL) {
    return uf;
  }
  uf->up = root(uf->up);
  return uf->up;
}

/**
 * @brief Wykonuje operacje Union z drzewa Union-Find na podanych drzewach.
 * @param[in,out] g wskaźnik na strukturę przechowującą stan gry.
 * @param[in,out] uf1 wskaźnik na pierwsze drzewo.
 * @param[in,out] uf2 wskaźnik na drugie drzewo.
 */
static void gamma_union_areas(gamma_t *g, union_find_t *uf1, union_find_t *uf2) {
  union_find_t *root1 = root(uf1);
  union_find_t *root2 = root(uf2);
  if (root1 == root2) {
    return;
  }

  union_find_t *new_root;
  union_find_t *other;

  if (root1->rank > root2->rank) {
    new_root = root1;
    other = root2;
  } else {
    new_root = root2;
    other = root1;
  }
  if (new_root->rank == other->rank) {
    new_root->rank++;
  }

  other->up = new_root;

  g->total_areas--;
}

/**
 * @brief Uaktualnia liczbę obszarów danego gracza po postawieniu pionka na dane pole,
 * wykonuje operację Union podanego pola z sąsiadami, które należą do tego samego gracza.
 * @param[in,out] g wskaźnik na strukturę przechowującą stan gry.
 * @param[in] player numer gracza.
 * @param[in] x pierwsza współrzędna.
 * @param[in] y druga współrzędna.
 */
static void gamma_update_areas(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
  uint64_t prev_areas = g->total_areas;
  g->total_areas++;

  for (int i = 0; i < NEIGHBOURS_COUNT; i++) {
    int64_t nx = (int64_t) x + NEIGHBORS[i][0];
    int64_t ny = (int64_t) y + NEIGHBORS[i][1];
    if (valid_coords(g, nx, ny)) {
      if (g->board[x][y].player == g->board[nx][ny].player) {
        gamma_union_areas(g, &g->board[x][y], &g->board[nx][ny]);
      }
    }
  }

  g->players[player].areas += g->total_areas - prev_areas;
}

/**
 * @brief Uaktualnia liczę pól graczy po postawieniu przez podanego gracza pionka na danym polu.
 * @param[in,out] g wskaźnik na strukturę przechowującą stan gry.
 * @param[in] player numer gracza.
 * @param[in] x pierwsza współrzędna.
 * @param[in] y druga współrzędna.
 * @param[in] prev_player gracz, który wcześniej okupował dane pole lub @p EMPTY_FIELD jeżeli było puste.
 */
static void gamma_update_free_fields(gamma_t *g,
                                     uint32_t player,
                                     uint32_t x,
                                     uint32_t y,
                                     uint32_t prev_player) {
  int64_t neighbors[] = {-1, -1, -1, -1};
  for (int i = 0; i < NEIGHBOURS_COUNT; i++) {
    int64_t nx = (int64_t) x + NEIGHBORS[i][0];
    int64_t ny = (int64_t) y + NEIGHBORS[i][1];
    if (valid_coords(g, nx, ny)) {
      if (g->board[nx][ny].player == EMPTY_FIELD) {
        if (neighboring_player_count(g, player, (uint32_t) nx, (uint32_t) ny) == 1) {
          g->players[player].free_fields++;
        }
        if (neighboring_player_count(g, prev_player, (uint32_t) nx, (uint32_t) ny) == 0) {
          g->players[prev_player].free_fields--;
        }
      }
      bool was_before = false;
      for (int j = 0; j < i; j++) {
        if (neighbors[j] == g->board[nx][ny].player) {
          was_before = true;
          break;
        }
      }
      if (!was_before) {
        neighbors[i] = g->board[nx][ny].player;
        if (prev_player == EMPTY_FIELD) {
          g->players[g->board[nx][ny].player].free_fields--;
        }
      }
    }
  }
}

bool gamma_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
  if (!(validate_board(g) && validate_move(g, player, x, y))) {
    return false;
  }

  if (g->board[x][y].player != EMPTY_FIELD) {
    return false;
  }

  gamma_set_player(g, player, x, y);

  gamma_update_areas(g, player, x, y);
  gamma_update_free_fields(g, player, x, y, EMPTY_FIELD);

  return true;
}

/** @brief Sprawdza, czy gracz może wykonać złoty ruch.
 * Sprawdza, czy gracz @p player jeszcze nie wykonał w tej rozgrywce złotego
 * ruchu i jest przynajmniej jedno pole zajęte przez innego gracza.
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia nie większa od wartości
 *                      @p players z funkcji @ref gamma_new.
 * @return Wartość @p true, jeśli gracz jeszcze nie wykonał w tej rozgrywce
 * złotego ruchu i jest przynajmniej jedno pole zajęte przez innego gracza,
 * a @p false w przeciwnym przypadku.
 */
static inline bool check_golden_possibility(gamma_t *g, uint32_t player) {
  return g->occupied > g->players[player].busy_fields && !g->players[player].used_golden;
}

bool gamma_golden_possible(gamma_t *g, uint32_t player) {
  if (!(validate_board(g) && validate_player(g, player))) {
    return false;
  }

  if (check_golden_possibility(g, player)) {
    if (gamma_player_areas(g, player) < gamma_max_areas(g)) {
      return true;
    }
    for (uint32_t x = 0; x < g->width; x++) {
      for (uint32_t y = 0; y < g->height; y++) {
        uint32_t prev_player = gamma_player(g, x, y);
        if (prev_player == player
            || prev_player == EMPTY_FIELD
            || neighboring_player_count(g, player, x, y) == 0) {
          continue;
        }

        if (neighboring_player_count(g, prev_player, x, y)
            <= gamma_max_areas(g) - gamma_player_areas(g, prev_player) + 1) {
          return true;
        }

        if (gamma_golden_move(g, player, x, y)) {
          bool prev_player_golden = g->players[prev_player].used_golden;
          g->players[prev_player].used_golden = false;
          gamma_golden_move(g, prev_player, x, y);

          g->players[player].used_golden = false;
          g->players[prev_player].used_golden = prev_player_golden;
          return true;
        }
      }
    }

  }

  return false;
}

/**
 * @brief Resetuje rozłącza drzewa Union-Find.
 * Ustawia liczbę obszarów na liczbę zajętych pól.
 * @param[in,out] g wskaźnik na strukturę przechowującą stan gry
 */
static inline void gamma_clear_areas(gamma_t *g) {
  for (uint32_t x = 0; x < g->width; x++) {
    for (uint32_t y = 0; y < g->height; y++) {
      g->board[x][y].rank = 0;
      g->board[x][y].up = NULL;
    }
  }
  g->total_areas = g->occupied;
}

/**
 * @brief Od nowa wykonuje łączy pola w obszary na planszy wcześniej je rozłączając.
 * Aktualizuję łączną liczbę obszarów.
 * @param[in, out] g wskaźnik na
 */
static void gamma_remake_areas(gamma_t *g) {
  gamma_clear_areas(g);
  for (uint32_t x = 0; x < g->width; x++) {
    for (uint32_t y = 0; y < g->height; y++) {
      uint32_t player = g->board[x][y].player;
      if (player == EMPTY_FIELD) {
        continue;
      }
      for (int i = 0; i < 2; i++) {
        int64_t nx = (int64_t) x + NEIGHBORS[i][0];
        int64_t ny = (int64_t) y + NEIGHBORS[i][1];
        if (valid_coords(g, nx, ny)) {
          if (g->board[nx][ny].player == player) {
            gamma_union_areas(g, &g->board[nx][ny], &g->board[x][y]);
          }
        }
      }
    }
  }
}

/**
 * @brief Zdejmuje pionek z podanego pola, zmienia liczbę obszarów,
 * gracza który na nim był, ale nie zmienia liczby dostępnych pól dla graczy.
 * Zmniejsza łączną liczbę zajętych pól jak i pól zajętych przez gracza,
 * który wcześniej zajmował pole o 1.
 * @param[in,out] g wskaźnik na strukturę przechowującą stan gry
 * @param[in] x pierwsza współrzędna
 * @param[in] y druga współrzędna
 */
static void gamma_leave_field(gamma_t *g, uint32_t x, uint32_t y) {
  uint32_t prev_player = g->board[x][y].player;
  uint64_t prev_areas = g->total_areas;

  gamma_set_player(g, EMPTY_FIELD, x, y);

  gamma_remake_areas(g);

  g->players[prev_player].areas += g->total_areas - prev_areas;
}

bool gamma_golden_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
  if (!(validate_board(g) && validate_move(g, player, x, y)
      && check_golden_possibility(g, player))) {
    return false;
  }

  uint32_t prev_player = g->board[x][y].player;
  if (prev_player == EMPTY_FIELD || prev_player == player) {
    return false;
  }

  gamma_leave_field(g, x, y);

  if (g->players[prev_player].areas > g->max_areas) {
    gamma_set_player(g, prev_player, x, y);

    gamma_update_areas(g, prev_player, x, y);

    return false;
  } else {
    gamma_set_player(g, player, x, y);

    gamma_update_areas(g, player, x, y);
    gamma_update_free_fields(g, player, x, y, prev_player);

    g->players[player].used_golden = true;
  }

  return true;
}

uint64_t gamma_busy_fields(const gamma_t *g, uint32_t player) {
  if (!(validate_board(g) && validate_player(g, player))) {
    return false;
  }

  return g->players[player].busy_fields;
}

uint64_t gamma_free_fields(const gamma_t *g, uint32_t player) {
  if (!(validate_board(g) && validate_player(g, player))) {
    return false;
  }

  if (g->players[player].areas < g->max_areas) {
    return (uint64_t) g->width * (uint64_t) g->height - g->occupied;
  }
  return g->players[player].free_fields;
}

/**
 * @brief Zwraca liczbę cyfr w danej liczbie. Lub 0 jeżeli podane jest 0.
 * @param[in] num liczba.
 * @return Liczba cyfr w podanej liczbie.
 */
static inline int number_of_digits(uint32_t num) {
  int count = 0;
  while (num > 0) {
    count++;
    num /= 10;
  }
  return count;
}

inline int gamma_board_cell_size(const gamma_t *g) {
  int size = number_of_digits(g->players_count);
  return size == 1 ? size : size + 1;
}

char *gamma_board(const gamma_t *g) {
  if (!validate_board(g)) {
    return NULL;
  }

  int cell_size = gamma_board_cell_size(g);
  uint64_t curr_char = 0;
  uint64_t size =
      (uint64_t) g->width * (uint64_t) g->height * (uint64_t) cell_size + (uint64_t) g->height + 1;
  char *board = malloc(sizeof(char) * size);
  if (board == NULL) {
    return NULL;
  }
  for (uint32_t y = g->height; y > 0; y--) {
    for (uint32_t x = 0; x < g->width; x++) {
      uint32_t player = g->board[x][y - 1].player;
      if (player == EMPTY_FIELD) {
        sprintf_s(board + curr_char, "%*s", cell_size, ".");
      } else {
        sprintf_s(board + curr_char, "%*" PRIu32, cell_size, player);
      }
      curr_char += cell_size;
    }
    board[curr_char] = '\n';
    curr_char++;
  }
  board[curr_char] = '\0';
  return board;
}

inline uint32_t gamma_players(const gamma_t *g) {
  return g->players_count;
}

inline uint32_t gamma_height(const gamma_t *g) {
  return g->height;
}

inline uint32_t gamma_width(const gamma_t *g) {
  return g->width;
}
inline uint32_t gamma_max_areas(const gamma_t *g) {
  return g->max_areas;
}

inline bool gamma_has_golden(const gamma_t *g, uint32_t player) {
  return !g->players[player].used_golden;
}

inline uint32_t gamma_player(const gamma_t *g, uint32_t x, uint32_t y) {
  return g->board[x][y].player;
}

inline uint32_t gamma_player_areas(const gamma_t *g, uint32_t player) {
  return (uint32_t) g->players[player].areas;
}