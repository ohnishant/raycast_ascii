#define _DEFAULT_SOURCE
#define _GNU_SOURCE
#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "terminal.h"

#define MAP_TILE_SIZE 64

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

int getCursorPosition(int* rows, int* cols) {
    char buf[32];
    unsigned int i = 0;

    if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) {
        return -1;
    }
    printf("\r\n");

    while (i < sizeof(buf) - 1) {
        if (read(STDIN_FILENO, &buf[i], 1) != 1) {
            break;
        }
        if (buf[i] == 'R') {
            break;
        }
        ++i;
    }
    buf[i] = '\0';
    if (buf[0] != '\x1b' || buf[1] != '[') {
        return -1;
    }
    if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) {
        return -1;
    }

    termReadKey();
    return 0;
}

int getScreenSize(int* rows, int* cols) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {

        /*fprintf(stderr, "[WARN]:\tFailed to get window size... Running Fallback\r\n");*/
        /**/
        /*if (write(STDIN_FILENO, "\x1b[999C\x1b[999B", 12) != 12) {*/
        /*    fprintf(stderr, "[FATAL]:\tFailed to get window size... Fallback failed\r\n");*/
        /*    return -1;*/
        /*}*/
        getCursorPosition(rows, cols);
        fprintf(stderr, "[INFO]:\tFallabck Successful, Rows: %d, Cols: %d\r\n", *rows, *cols);
        return 0;
    } else {
        *rows = ws.ws_row;
        *cols = ws.ws_col;
        fprintf(stderr, "[INFO]:\tGot Window size successfully, Rows: %d, Cols: %d\r\n", *rows, *cols);
        return 0;
    }
}

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
        write(STDOUT_FILENO, "THING RAN\r\n", 11);
        write(STDOUT_FILENO, posInStr, 8);
        write(STDOUT_FILENO, "\r\n", 2);
    }
    return 0;
}
