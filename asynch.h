#ifndef ASYNCH_H
#define ASYNCH_H
#include "model.h"

/* handles user depressing spacebar in game */
void handleJump(Model *model);
/* handles user depressing '1' in menu */
void handle1p(Model *model);
/* handles user depressing '2' in menu */
void handle2p(Model *model);
/* handles user depressing spacebar in game over screen */
void handleRetry(Model *model);
/* handles user depressing 'q' in game over screen */
void handleQuitToMenu(Model *model);

#endif