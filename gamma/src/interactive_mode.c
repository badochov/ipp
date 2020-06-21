/** @file
 * Implementacja obsługi trybu interpretowanego.
 *
 * @author Hubert Badocha <h.badocha@student.uw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 28.04.2020
 */

#include "interactive_mode.h"

#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/ioctl.h>

/** @brief  Początek sekwencji specjalnej ANSI. */
#define ESC_CODE "\033["
/** @brief  Znak escape. */
#define ESC ESC_CODE[0]
/** @brief  Znak końca transmisji. */
#define EOT '\4'

/** @brief  Sekwencja specjalna ANSI czyszcząca ekran.*/
#define CLEAR_SCREEN ESC_CODE "2J"
/** @brief  Sekwencja specjalna ANSI chowająca kursor. */
#define HIDE_CURSOR ESC_CODE "?25l"
/** @brief  Sekwencja specjalna ANSI pokazująca kursor. */
#define SHOW_CURSOR ESC_CODE "?25h"
/** @brief  Sekwencja specjalna ANSI resetująca formatowanie tekstu. */
#define RESET_TEXT_FORMAT ESC_CODE "0m"
/** @brief  Sekwencja specjalna ANSI wracająca na początek ekranu. */
#define HOME ESC_CODE "H"

/** @brief  Trzeci znak w sekwencji wysłanej na wejście po wciśnięciu strzałki w górę. */
#define ARROW_UP 'A'
/** @brief  Trzeci znak w sekwencji wysłanej na wejście po wciśnięciu strzałki w dół. */
#define ARROW_DOWN 'B'
/** @brief  Trzeci znak w sekwencji wysłanej na wejście po wciśnięciu strzałki w prawo. */
#define ARROW_RIGHT 'C'
/** @brief  Trzeci znak w sekwencji wysłanej na wejście po wciśnięciu strzałki w lewo. */
#define ARROW_LEFT 'D'
/** @brief  Drugi znak w sekwencji wysłanej na wejście po wciśnięciu strzałki. */
#define PRE_ARROW_CHARACTER ESC_CODE[1]

/** @brief  Stała pomagająca ustawić tekst na pogrubiony. */
#define BOLD "1"
/** @brief  Stała pomagająca ustawić kolor tła na biały. */
#define WHITE_BACKGROUND "47"
/** @brief  Stała pomagająca ustawić kolor czcionki na biały. */
#define BLACK_TEXT "30"
/** @brief  Stała pomagająca ustawić kolor czcionki na czerwony. */
#define RED_TEXT "31"
/** @brief  Stała pomagająca ustawić kolor czcionki na żółty. */
#define YELLOW_TEXT "33"
/** @brief  Stała pomagająca ustawić kolor czcionki na zielony. */
#define GREEN_TEXT "32"

/** @brief Stałą pomagająca ustawić kolor czcionki na kolor obecnego gracza. */
#define CURRENT_PLAYER_TEXT_COLOR RED_TEXT

/** @brief Stała mówiąca ile dodatkowych linii jest wyświetlana podczas rozgrywki oprócz planszy. */
#define ADDITIONAL_LINES 8

/**
 * @brief Sprawdza, czy podany gracz może wykonać ruch,
 * czyli ma wolne pole lub dostępny złoty ruch.
 * @param[in] g wskaźnik na strukturę przetrzymującą stan gry.
 * @param[in] player numer gracza.
 * @return @p true jeżeli może wykonać, @p false w przeciwnym przypadku.
 */
static inline bool can_move(gamma_t *g, uint32_t player) {
  return gamma_free_fields(g, player) != EMPTY_FIELD || gamma_golden_possible(g, player);
}

/**
 * @brief Zwraca drugą współrzędną odbitą względem środka planszy.
 * Zakłada, że @p y jest mniejszy od wysokości planszy.
 * @param[in] g wskaźnik na strukturę przetrzymującą stan gry.
 * @param[in] y druga współrzędna.
 * @return @p y odbita względem środka planszy.
 */
static inline uint32_t get_real_y(const gamma_t *g, uint32_t y) {
  return gamma_height(g) - 1 - y;
}

/**
 * @brief Ustawia kolor czcionki i tła na podane.
 * @param[in] text_color kolor czcionki zgodny ze specyfikacją ANSI.
 * @param[in] background_color kolor tła zgodny ze specyfikacją ANSI,
 * lub @p "" -> pusty string aby pominąć zmianę koloru tła.
 */
static inline void set_color(char *text_color, char *background_color) {
  printf(ESC_CODE "%s", text_color);
  if (background_color[0] != '\0') {
    printf(";%s", background_color);
  }
  printf("m");
}

/**
 * @brief Ustawia czcionkę na pogrubioną.
 */
static inline void bold() {
  printf(ESC_CODE BOLD "m");
}

/**
 * @brief Resetuje formatowanie tekstu.
 */
static inline void reset_formatting() {
  printf(RESET_TEXT_FORMAT);
}

/**
 * @brief Wypisuje dane o podanym graczu.
 * @param[in] g wskaźnik na strukturę przetrzymującą stan gry.
 * @param[in] player numer gracza.
 */
static inline void print_player_string(const gamma_t *g, uint32_t player) {
  uint64_t busy = gamma_busy_fields(g, player);
  uint64_t free = gamma_free_fields(g, player);
  uint32_t areas = gamma_player_areas(g, player);
  uint32_t max_areas = gamma_max_areas(g);

  bold();
  printf("Player %" PRIu32 "\n", player);
  reset_formatting();

  printf("Busy fields: %" PRIu64 "\n", busy);
  printf("Available fields: %" PRIu64 "\n", free);
  printf("Areas: %" PRIu32 " / %" PRIu32 "\n", areas, max_areas);

  if (gamma_has_golden(g, player)) {
    set_color(YELLOW_TEXT, "");
    puts("Has golden move");
    reset_formatting();
  }
}

/**
 * @brief Wypisuję stan końcowy planszy i podsumowanie graczy oraz zwycięzcę.
 * @param[in] g wskaźnik na strukturę przetrzymującą stan gry.
 * @return @p 1 jeżeli wystąpił błąd krytyczny, @p 0 w przeciwnym przypadku.
 */
static inline int print_final_state(const gamma_t *g) {
  char *board = gamma_board(g);
  if (board == NULL) {
    return EXIT_FAILURE;
  }
  bold();
  printf("%s\n", board);
  reset_formatting();
  free(board);

  uint64_t max_busy_fields = 0;
  uint64_t max_busy_fields_players = 0;
  for (uint32_t player = 0; player++ < gamma_players(g);) {
    uint64_t busy = gamma_busy_fields(g, player);
    if (busy == max_busy_fields) {
      max_busy_fields_players++;
    } else if (busy > max_busy_fields) {
      max_busy_fields = busy;
      max_busy_fields_players = 1;
    }

    bold();
    printf("PLAYER %" PRIu32 "\n", player);
    reset_formatting();
    printf("\t Busy fields: %" PRIu64 "\n", busy);
  }

  puts("");
  bold();
  puts(max_busy_fields_players > 1 ? "Winners:" : "Winner:");
  reset_formatting();

  for (uint32_t player = 0; player++ < gamma_players(g);) {
    uint64_t busy = gamma_busy_fields(g, player);
    if (busy == max_busy_fields) {
      bold();
      printf("\tPLAYER %" PRIu32 "\n", player);
      reset_formatting();
    }
  }

  bold();
  set_color(GREEN_TEXT, "");
  puts("\nCongratulations!");
  reset_formatting();

  return EXIT_SUCCESS;
}

/**
 * @brief Funkcja sprawdzająca czy na standardowe wejście została podana strzałka.
 * Jeżeli została podana zmienia odpowiednio podane @p y i @p x.
 * @param[in] ch wczytany znak.
 * @param[in,out] x wskaźnik na pierwszą współrzędną.
 * @param[in,out] y wskaźnik na drugą współrzędną.
 * @param[in] g wskaźnik na strukturę przechowującą stan gry.
 * @return @p true jeżeli wczytano strzałkę, @p false w przeciwnym przypadku.
 */
static inline bool handle_arrows(int ch, uint32_t *x, uint32_t *y, const gamma_t *g) {
  if (ch == ESC) {
    int next_ch = getchar();
    if (next_ch == PRE_ARROW_CHARACTER) {
      int arrow_ch = getchar();
      switch (arrow_ch) {
        case ARROW_UP:
          *y = *y == 0 ? *y : *y - 1;
          return true;
        case ARROW_DOWN:
          *y = *y == gamma_height(g) - 1 ? *y : *y + 1;
          return true;
        case ARROW_RIGHT:
          *x = *x == gamma_width(g) - 1 ? *x : *x + 1;
          return true;
        case ARROW_LEFT:
          *x = *x == 0 ? *x : *x - 1;
          return true;
        default:
          ungetc(arrow_ch, stdin);
      }
    }
    ungetc(next_ch, stdin);
  }
  return false;
}

/**
 * @brief Wykonuję akcję.
 * @param[in] ch wczytany znak.
 * @param[in] player numer gracza.
 * @param[in,out] g wskaźnik na strukturę przechowującą stan gry.
 * @param[in,out] x wskaźnik na pierwszą współrzędną.
 * @param[in,out] y wskaźnik na drugą współrzędną.
 * @return Wartości typu wyliczeniowego zgodnie ze specyfikacją.
 */
static enum {
  END_MOVE, ///< Ruch danego gracza zakończył się.
  END_GAME, ///< Gra ma zostać zakończona.
  NO_ACTION, ///< Wczytany znak nie ma przypisanej instrukcji.
  WRONG_MOVE, ///< Ruch, który próbował zrobić gracz był niepoprawny.
  MOVE_CURSOR, ///< Akcja polegała na zmianie miejsca kursora.
  ERROR ///< Napotkano na błąd krytyczny.
} action(int ch,
         uint32_t player,
         gamma_t *g,
         uint32_t *x,
         uint32_t *y) {
  if (handle_arrows(ch, x, y, g)) {
    return MOVE_CURSOR;
  }
  switch (ch) {
    case EOT:
      return END_GAME;
    case EOF:
      return ERROR;
    case 'c':
    case 'C':
      return END_MOVE;
    case 'g':
    case 'G':
      if (gamma_golden_move(g, player, *x, get_real_y(g, *y))) {
        return END_MOVE;
      }
      return WRONG_MOVE;
    case ' ':
      if (gamma_move(g, player, *x, get_real_y(g, *y))) {
        return END_MOVE;
      }
      return WRONG_MOVE;
    default:
      break;
  }

  return NO_ACTION;
}

/**
 * @brief Wypisuje podane pole.
 * @param[in] g wskaźnik na strukturę przetrzymującą stan gry.
 * @param[in] x pierwsza współrzędna.
 * @param[in] y druga współrzędna.
 */
static inline void print_field(const gamma_t *g, uint32_t x, uint32_t y) {
  uint32_t player = gamma_player(g, x, y);
  int width = gamma_board_cell_size(g);

  if (player == EMPTY_FIELD) {
    printf("%*s", width, ".");
  } else {
    printf("%*" PRIu32, width, player);
  }
}

/**
 * @brief Wypisuje planszę.
 * @param[in] g wskaźnik na strukturę przetrzymującą stan gry.
 * @param[in] cursor_x pierwsza współrzędna pozycji kursora.
 * @param[in] cursor_y druga współrzędna pozycji kursora.
 * @param[in] player numer obecnego gracza.
 */
static inline void print_board(const gamma_t *g,
                               uint32_t cursor_x,
                               uint32_t cursor_y,
                               uint32_t player) {
  for (uint32_t helper_y = 0; helper_y < gamma_height(g); helper_y++) {
    for (uint32_t x = 0; x < gamma_width(g); x++) {
      uint32_t y = get_real_y(g, helper_y);
      if (helper_y == cursor_y && x == cursor_x) {
        set_color(BLACK_TEXT, WHITE_BACKGROUND);
      }
      if (gamma_player(g, x, y) == player && player != EMPTY_FIELD) {
        set_color(CURRENT_PLAYER_TEXT_COLOR, "");
      }
      print_field(g, x, y);
      reset_formatting();
    }
    puts("");
  }
}

/**
 * @brief Czyści ekran.
 */
static inline void clear_screen() {
  printf(CLEAR_SCREEN);
  printf(HOME);
}

/**
 * @brief Wypisuje podaną wiadomość o błędzie.
 * @param[in] string wiadomość o błędzie.
 */
static inline void display_error_message(const char *string) {
  bold();
  set_color(RED_TEXT, "");

  printf("%s", string);
  reset_formatting();
}

/**
 * @brief Zmienia ustawienia terminalu, tak aby getchar nie czekał na koniec linii.
 * Dodatkowo ukrywa kursor.
 * @param[in,out] old_t wskaźnik na ustawienia terminalu,
 * który będzie przechowywał poprzednie ustawienia terminalu.
 * @param[in,out] new_t wskaźnik na ustawienie terminalu,
 * który będzie przechowywał nowe ustawienia terminalu.
 * @return @p true jeżeli udało zmienić się ustawienia
 * lub dane wczytywane są z przekierowania,
 * @p false w przeciwnym przypadku.
 */
static inline bool change_terminal_settings(struct termios *old_t, struct termios *new_t) {
  if (isatty(STDIN_FILENO)) {
    if (tcgetattr(STDIN_FILENO, old_t)) {
      return false;
    }
    *new_t = *old_t;
    new_t->c_lflag &= ~((uint32_t) ICANON | (uint32_t) ECHO);
    if (tcsetattr(STDIN_FILENO, TCSANOW, new_t)) {
      return false;
    }
    printf(HIDE_CURSOR);
  }

  return true;
}

/**
 * @brief Przywraca poprzednie ustawienia terminalu oraz pokazuje kursor.
 * @param[in,out] old_t poprzednia ustawienia terminalu.
 * @param[in,out] exit_code wskaźnik na kod wyjścia programu,
 * w przypadku błędu ustawiany jest na 1.
 */
static inline void restore_terminal_settings(struct termios old_t, int *exit_code) {
  if (isatty(STDIN_FILENO)) {
    if (tcsetattr(STDIN_FILENO, TCSANOW, &old_t)) {
      *exit_code = EXIT_FAILURE;
    }
    printf(SHOW_CURSOR);
  }
}

/**
 * @brief Sprawdza, czy szerokość terminalu jest wystarczająca aby zmieścić rozgrywkę.
 * @param[in] g wskaźnik na strukturę przechowującą stan gry.
 * @return @p true jeżeli rozgrywka się zmieści, @p false w przeciwnym przypadku.
 */
static inline bool check_terminal_dimensions(const gamma_t *g) {
  if (isatty(STDOUT_FILENO)) {
    struct winsize w;
    if (!ioctl(STDOUT_FILENO, TIOCGWINSZ, &w)) {
      return (w.ws_col >= (uint64_t) gamma_width(g) * (uint64_t) gamma_board_cell_size(g))
          && w.ws_row >= (uint64_t) gamma_height(g) + ADDITIONAL_LINES;
    }
  }

  return true;
}

int interactive_mode_start(gamma_t *g) {
  static struct termios old_t, new_t;

  int exit_code = EXIT_SUCCESS;

  if (!change_terminal_settings(&old_t, &new_t)) {
    return EXIT_FAILURE;
  }
  if (!check_terminal_dimensions(g)) {
    display_error_message("Terminal is to small for this game.");
    restore_terminal_settings(old_t, &exit_code);
    return EXIT_FAILURE;
  }

  bool somebody_can_move = true;
  bool end = false;
  bool end_move;
  bool error_is_displayed = true;

  uint32_t x = 0;
  uint32_t y = 0;

  clear_screen();
  while (somebody_can_move && !end) {
    somebody_can_move = false;
    for (uint32_t player = 0; player++ < gamma_players(g);) {
      if (can_move(g, player)) {
        somebody_can_move = true;

        end_move = false;
        bool print = true;
        while (!end_move) {
          if (print) {
            clear_screen();

            print_board(g, x, y, player);
            puts("");
            print_player_string(g, player);
            print = error_is_displayed = false;
          }

          switch (action(getchar(), player, g, &x, &y)) {
            case END_GAME:
              end = end_move = true;
              break;
            case END_MOVE:
              end_move = true;
              break;
            case NO_ACTION:
              break;
            case MOVE_CURSOR:
              print = true;
              break;
            case WRONG_MOVE:
              if (!error_is_displayed) {
                display_error_message("\nThis move is not possible!");
                error_is_displayed = true;
              }
              break;
            case ERROR:
              end_move = end = true;
              exit_code = EXIT_FAILURE;
              break;
          }
        }
        if (end) {
          break;
        }
      }
    }
  }

  restore_terminal_settings(old_t, &exit_code);
  clear_screen();

  return exit_code || print_final_state(g);
}
