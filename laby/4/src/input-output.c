#include "input-output.h"
#include <stdio.h>

extern int readInt()
{
    int a;
    scanf("%d", &a);
    return a;
}

extern void printUint(unsigned int a)
{
    printf("%du", a);
}