#define _DEFAULT_SOURCE
#define _GNU_SOURCE
#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <termio.h>
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

struct GameState {
    float playerX;
    float playerY;
    float deltaX;
    float deltaY;

    float cameraAngle;
    int screenRows;
    int screenCols;
};
struct GameState GS = { 0 };

void clearScreen() {
    write(STDOUT_FILENO, "\x1b[?25l", 6);
    write(STDOUT_FILENO, "\x1b[H", 3);
}
int termReadKey() {
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        /*if (nread == -1 && errno != EAGAIN)*/
        /*    die("read");*/
    }
    if (c == '\x1b') {
        char seq[3];
        if (read(STDIN_FILENO, &seq[0], 1) != 1)
            return '\x1b';
        if (read(STDIN_FILENO, &seq[1], 1) != 1) {
            return '\x1b';
        }

        /*if (seq[0] == '[') {*/
        /*    switch (seq[1]) {*/
        /*    case 'A':*/
        /*        return KEY_MOVE_UP;*/
        /*    case 'B':*/
        /*        return KEY_MOVE_DOWN;*/
        /*    case 'C':*/
        /*        return KEY_MOVE_RIGHT;*/
        /*    case 'D':*/
        /*        return KEY_MOVE_LEFT;*/
        /*    }*/
        /*}*/
        return '\x1b';
    } else {
        return c;
    }
}

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

struct termios original_termios = { 0 };

void disableRawMode() {
    fprintf(stderr, "[INFO]:\tDisabling Raw Mode\r\n");
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios) == -1) {
        /*die("tcsetattr");*/
    }
}

void enableRawMode() {
    fprintf(stderr, "[INFO]:\tEnabling Raw Mode\r\n");
    if (tcgetattr(STDIN_FILENO, &original_termios) == -1) {
        /*die("tcgetattr");*/
    }
    atexit(disableRawMode);

    struct termios raw = original_termios;

    raw.c_iflag &= ~(BRKINT | INPCK | ISTRIP | ICRNL | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    // 0 characters needed before read() can return
    raw.c_cc[VMIN] = 0;
    // 1/10 seconds needed before read() can return
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        /*die("tcsetattr");*/
    }
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

void termProcessKeyPress() {
    char c = termReadKey();
    switch (c) {
    case 'q':
        exit(0);
    case 'w':
        GS.playerX = GS.playerX + 0.1;
        break;
    case 's':
        GS.playerX = GS.playerX - 0.1;
        break;
    default:
        return;
    }
}

int main() {
    initGameState(&GS);
    enableRawMode();
    char posInStr[8];

    while (1) {
        termProcessKeyPress();
        strfromf(posInStr, 8, "%f", GS.playerX);
        write(STDOUT_FILENO, "THING RAN\r\n", 11);
        write(STDOUT_FILENO, posInStr, 8);
        write(STDOUT_FILENO, "\r\n", 2);
    }
    return 0;
}
