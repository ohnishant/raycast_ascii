#ifndef _RAYCAST_TERM_H
#define _RAYCAST_TERM_H

#include "nob.h"
#include "objects.h"

#include <termio.h>
#include <unistd.h>

void clearScreen(void);

void disableRawMode(void);
void enableRawMode(void);

int getCursorPosition(int* rows, int* cols);
int termReadKey(void);
int getScreenSize(int* rows, int* cols);

void termProcessKeyPress(struct GameState*);

#endif /* ifndef _RAYCAST_TERM_H */
