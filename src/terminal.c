#ifndef _TERM_H
#define _TERM_H

#include "nob.h"
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "objects.h"
#include "terminal.h"
#include "utils.h"

/// Config for the terminal before raw mode was turned on so it can be reverted
struct termios original_termios = { 0 };

void clearScreen(void) {
    write(STDOUT_FILENO, "\x1b[?25l", 6);
    write(STDOUT_FILENO, "\x1b[H", 3);
}

int termReadKey(void) {
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if (nread == -1 && errno != EAGAIN) {
            die("read");
        }
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

void termProcessKeyPress(struct GameState* GS) {
    char c = termReadKey();
    switch (c) {
    case 'q':
        exit(0);
    case 'w':
        GS->playerX = GS->playerX + 0.1;
        break;
    case 's':
        GS->playerX = GS->playerX - 0.1;
        break;
    default:
        return;
    }
}

void disableRawMode(void) {
    fprintf(stderr, "[INFO]:\tDisabling Raw Mode\r\n");
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios) == -1) {
        die("tcsetattr");
    }
}

void enableRawMode(void) {
    fprintf(stderr, "[INFO]:\tEnabling Raw Mode\r\n");
    if (tcgetattr(STDIN_FILENO, &original_termios) == -1) {
        die("tcgetattr");
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
        die("tcsetattr");
    }
}

#endif /* ifndef _TERM_H */
