#define _DEFAULT_SOURCE
#define _GNU_SOURCE
#define _BSD_SOURCE

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include "terminal.h"
#include "utils.h"

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
    GS->p.playerX = 86;
    GS->p.playerY = 86;
}

void* _termProcessKeyPress(void* gameState) {
    struct GameState* GS = (struct GameState*)gameState;
    while (1) {
        termProcessKeyPress(GS);
    }
}

int main(void) {
    initGameState(&GS);
    enableRawMode();
    char posInStr[8];

    pthread_t keyboardInputThread;
    pthread_create(&keyboardInputThread, NULL, &_termProcessKeyPress, &GS);

    while (1) {
        strfromf(posInStr, 8, "%.3f", GS.p.playerX);
        write(STDERR_FILENO, posInStr, 8);
        write(STDERR_FILENO, "\r\n", 2);

        // ~30 fps
        msleep(33);
    }
    pthread_join(keyboardInputThread, NULL);
    return 0;
}
