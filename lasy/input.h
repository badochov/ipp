//
// Created by badochov on 02.03.2020.
//

#ifndef LASY__INPUT_H_
#define LASY__INPUT_H_

#include <stdbool.h>

// wczytuje string z in do s
// zwraca -1 jeśli napotkał na nie whitespace o kodzie ascii mniejszy niż -1
//         0 jeśli nie na nic do wczytania
//         1 jeśli wczytanie przbiegło pomyślnie
short readString(char **s);

short readExtra();

short readCommand(char **s);

bool reachEOL();

#endif //LASY__INPUT_H_
