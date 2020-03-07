#include <stdio.h>
#include <stdlib.h>

static void sortuj(int *tablica, size_t n) {
  size_t i, j;
  int t;

  for (i = 0; i < n; ++i)
    for (j = 0; j < i; ++j)
      if (tablica[i] < tablica[j])
        t = tablica[i], tablica[i] = tablica[j], tablica[j] = t;
}

size_t ROZMIAR_TABLICY = 10;

int main() {
  size_t i, n;
  int *tablica = malloc(ROZMIAR_TABLICY * sizeof(int));

  n = 0;
  while (scanf("%d", &tablica[n]) == 1){
    ++n;
    if(n == ROZMIAR_TABLICY){
        ROZMIAR_TABLICY*=2;
        tablica = realloc(tablica, ROZMIAR_TABLICY * sizeof(int));
    }
  }

  sortuj(tablica, n);

  for (i = 0; i < n; ++i)
    printf("%d\n", tablica[i]);

  return 0;
}