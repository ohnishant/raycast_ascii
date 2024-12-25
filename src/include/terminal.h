#ifndef _RAYCAST_TERM_H
#define _RAYCAST_TERM_H

#include "nob.h"
#include "objects.h"

#include <termio.h>
#include <unistd.h>

void clearScreen(void);
int termReadKey(void);
void disableRawMode(void);
void enableRawMode(void);

void termProcessKeyPress(struct GameState*);

#endif /* ifndef _RAYCAST_TERM_H */
