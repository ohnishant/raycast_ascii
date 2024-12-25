#define _DEFAULT_SOURCE
#define _GNU_SOURCE
#define _BSD_SOURCE

#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "terminal.h"

int map[10][10] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 1, 1, 1, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

struct GameState GS = { 0 };

void initGameState(struct GameState* GS) {
    GS->playerX = 86;
    GS->playerY = 86;
}

int main(void) {
    initGameState(&GS);
    enableRawMode();
    char posInStr[8];

    while (1) {
        termProcessKeyPress(&GS);
        strfromf(posInStr, 8, "%f", GS.playerX);
        write(STDOUT_FILENO, posInStr, 8);
        write(STDOUT_FILENO, "\r\n", 2);
    }
    return 0;
}
