/** @file
 * Implementacja obsługi trybu wsadowego i wyboru trybu.
 *
 * @author Hubert Badocha <h.badocha@student.uw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 28.04.2020
 */

#include "batch_mode.h"
#include "gamma.h"
#include "interactive_mode.h"

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <inttypes.h>

/** @brief Maksymalna liczba argumentów podawanych w trybie wsadowym. */
#define MAX_ARGS_COUNT 4

/** @brief Poprawna liczba argumentów dla komendy rozpoczynającej grę. */
#define START_GAME_ARGS_COUNT 4
/** @brief Poprawna liczba argumentów dla komendy wywołującej gamma_move. */
#define GAMMA_MOVE_ARGS_COUNT 3
/** @brief Poprawna liczba argumentów dla komendy wywołującej gamma_golden_move. */
#define GAMMA_GOLDEN_MOVE_ARGS_COUNT 3
/** @brief Poprawna liczba argumentów dla komendy wywołującej gamma_busy_fields. */
#define GAMMA_BUSY_ARGS_COUNT 1
/** @brief Poprawna liczba argumentów dla komendy wywołującej gamma_free_fields. */
#define GAMMA_FREE_ARGS_COUNT 1
/** @brief Poprawna liczba argumentów dla komendy wywołującej gamma_board. */
#define GAMMA_BOARD_ARGS_COUNT 0
/** @brief Poprawna liczba argumentów dla komendy wywołującej gamma_golden_possible. */
#define GAMMA_GOLDEN_POSSIBLE_ARGS_COUNT 1

/** @brief  Znak komendy rozpoczynającej rozgrywkę w trybie wsadowym. */
#define BATCH_MODE 'B'
/** @brief Znak komendy rozpoczynającej rozgrywkę w trybie interaktywnym. */
#define INTERPRETED_MODE 'I'
/** @brief  Znak komendy wywołującej gamma_move. */
#define GAMMA_MOVE 'm'
/** @brief  Znak komendy wywołującej gamma_golden_move. */
#define GAMMA_GOLDEN_MOVE 'g'
/** @brief  Znak komendy wywołującej gamma_busy_fields. */
#define GAMMA_BUSY 'b'
/** @brief  Znak komendy wywołującej gamma_free_fields. */
#define GAMMA_FREE 'f'
/** @brief  Znak komendy wywołującej gamma_board. */
#define GAMMA_BOARD 'p'
/** @brief  Znak komendy wywołującej gamma_golden_possible. */
#define GAMMA_GOLDEN_POSSIBLE 'q'

/** @brief  Znak komentarza. */
#define COMMENT '#'

/**
 * @brief Wypisuje na standardowe wyjście diagnostyczne informacje o błędzie w danej linii.
 * @param[in] line_number numer linii.
 */
static inline void log_error(size_t line_number) {
  fprintf(stderr, "ERROR %zu\n", line_number);
}

/**
 * @brief Wykonuje wczytaną operacje.
 * @param[in,out] g wskaźnik na strukturę przechowującą stan gry.
 * @param[in] command wczytane polecenie.
 * @param[in] args tablica z wczytanymi argumentami.
 * @param[in] args_count liczba wczytanych argumentów.
 * @return @p true jeżeli wykonano pomyślnie operacje,
 * @p false jeżeli podane argumenty były niepoprawne.
 */
static bool perform_operation(gamma_t *g,
                              int command,
                              const uint32_t args[MAX_ARGS_COUNT],
                              int args_count) {
  if (command == GAMMA_MOVE) {
    if (args_count == GAMMA_MOVE_ARGS_COUNT) {
      printf("%d\n", gamma_move(g, args[0], args[1], args[2]));
      return true;
    }
  } else if (command == GAMMA_GOLDEN_MOVE) {
    if (args_count == GAMMA_GOLDEN_MOVE_ARGS_COUNT) {
      printf("%d\n", gamma_golden_move(g, args[0], args[1], args[2]));
      return true;
    }
  } else if (command == GAMMA_BUSY) {
    if (args_count == GAMMA_BUSY_ARGS_COUNT) {
      printf("%" PRIu64 "\n", gamma_busy_fields(g, args[0]));
      return true;
    }
  } else if (command == GAMMA_FREE) {
    if (args_count == GAMMA_FREE_ARGS_COUNT) {
      printf("%" PRIu64 "\n", gamma_free_fields(g, args[0]));
      return true;
    }
  } else if (command == GAMMA_GOLDEN_POSSIBLE) {
    if (args_count == GAMMA_GOLDEN_POSSIBLE_ARGS_COUNT) {
      printf("%d\n", gamma_golden_possible(g, args[0]));
      return true;
    }
  } else if (command == GAMMA_BOARD) {
    if (args_count == GAMMA_BOARD_ARGS_COUNT) {
      char *board = gamma_board(g);
      if (board == NULL) {
        return false;
      }
      printf("%s", board);
      free(board);
      return true;
    }
  }
  return false;
}

/**
 * @brief Sprawdza, czy linia powinna być zignorowana.
 * @param[in] command pierwszy znak linii.
 * @return @p true jeżeli linia powinna być zignorowana, @p false w przeciwnym przypadku.
 */
static inline bool ignore_line(int command) {
  return command == '\n' || command == COMMENT;
}

/**
 * @brief Typ wyliczeniowy mówiący o tym jak przebiegło dojście do końca linii.
 * Pomyślne dojście to takie, w którym napotkano tylko białe znaki.
 */
typedef enum {
  OK, /*!< Dojście do końca linii przebiegło pomyślnie. */
  ADDITIONAL_ARGS, /*!< Podczas dojścia do końca linii natrafiono na dodatkowe argumenty. */
  EOF_REACHED /*!< Podczas dojścia do końca linii natrafiono na koniec pliku. */
} reach_EOL_t;

/**
 * @brief Dochodzi do końca linii na standardowym wejściu.
 * @return Wartość typu wyliczeniowego zgodną z opisem.
 */
static inline reach_EOL_t reach_EOL() {
  int c = getchar();
  reach_EOL_t ret = OK;
  while (c != EOF && c != '\n') {
    if (!isspace(c)) {
      ret = ADDITIONAL_ARGS;
    }
    c = getchar();
  }
  if (c == EOF) {
    return EOF_REACHED;
  }
  return ret;
}

/**
 * @brief Przeprowadza konwersję znaku na liczbę, jeżeli podany czar jest cyfrą.
 * @param[in] c znak.
 * @return Wartość chara jako int, lub @p -1 jeżeli podany char nie jest cyfrą.
 */
static inline int char_to_int(int c) {
  if (c > '9' || c < '0') {
    return -1;
  }
  return c - '0';
}

/**
 * @brief Sprawdza, czy podany znak to znak końca lini bądź końca pliku.
 * @param[in] c znak.
 * @return @p true jeżeli znak to @p EOF lub @p EOL, @p false w przeciwnym przypadku.
 */
static inline bool is_EOL_or_EOF(int c) {
  return c == EOF || c == '\n';
}

/**
 * @brief Funkcja pomocnicza wczytująca znak ze standardowego wejścia.
 * Jeżeli wczytany znak to @p EOF lub @p EOL,
 * wstawia z powrotem na początek strumienia.
 * @return Wczytany znak.
 */
static inline int getch() {
  int c = getchar();
  if (is_EOL_or_EOF(c)) {
    ungetc(c, stdin);
  }

  return c;
}

/**
 * @brief Funkcja dochodząca do końca argumentu na standardowym wejściu.
 */
static inline void reach_end_of_argument() {
  while (true) {
    int c = getch();
    if (isspace(c) || c == EOF) {
      break;
    }
  }
}

/**
 * @brief Funkcja wczytująca argument.
 * @return Wczytany argument,
 * lub @p -1 jeżeli wczytany argument nie jest liczbą
 * lub nie jest w zakresie zmiennej typu uint_32t.
 */
static int64_t read_arg() {
  int64_t arg = 0;
  bool started = false;

  while (arg >= 0 && arg <= UINT32_MAX) {
    int c = getch();

    if (isspace(c)) {
      if (started) {
        break;
      }
      if (is_EOL_or_EOF(c)) {
        arg = -1;
        break;
      }
      continue;
    }

    int digit = char_to_int(c);

    if (digit < 0) {
      arg = -1;
    } else {
      arg = arg * 10 + digit;
      started = true;
    }
  }

  if (started && arg < 0) {
    reach_end_of_argument();
  }

  if (arg > UINT32_MAX) {
    return -1;
  }
  return arg;
}

/**
 * @brief Funkcja wczytująca argumenty.
 * @param[in,out] args tablica na argumenty.
 * @return Liczba wczytanych argumentów.
 */
static inline int get_args(uint32_t args[MAX_ARGS_COUNT]) {
  int count = 0;
  int c = getchar();
  ungetc(c, stdin);

  if (isspace(c)) {
    for (int i = 0; i < MAX_ARGS_COUNT; i++) {
      int64_t arg = read_arg();
      if (arg < 0) {
        break;
      }
      args[i] = (uint32_t) arg;
      count++;
    }
  }
  return count;
}

int batch_mode_start() {
  gamma_t *g = NULL;
  size_t line = 0;
  uint32_t args[MAX_ARGS_COUNT];
  int exit_code = EXIT_SUCCESS;
  bool board_initiated = false;

  while (true) {
    line++;

    int c = getch();
    int args_count = get_args(args);
    reach_EOL_t r = reach_EOL();

    if (ignore_line(c)) {
      continue;
    }

    if (r == EOF_REACHED) {
      if (c != EOF) {
        log_error(line);
      }
      break;
    }

    if (r == OK) {
      if (!board_initiated) {
        if (c == BATCH_MODE || c == INTERPRETED_MODE) {
          if (args_count == START_GAME_ARGS_COUNT) {
            g = gamma_new(args[0], args[1], args[2], args[3]);
            if (g != NULL) {
              if (c == INTERPRETED_MODE) {
                exit_code = interactive_mode_start(g);
                break;
              }
              printf("OK %zu\n", line);
              board_initiated = true;
              continue;
            }
          }
        }
      } else {
        if (perform_operation(g, c, args, args_count)) {
          continue;
        }
      }
    }
    log_error(line);
  }

  gamma_delete(g);
  return exit_code;
}
