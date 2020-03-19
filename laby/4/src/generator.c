unsigned int state = 0;
unsigned CONST = 8192;

extern void setState(unsigned newState)
{
    state = newState;
}

extern unsigned generateRandomUint()
{
    return ((state * CONST + CONST) * CONST) % (CONST * CONST);
}