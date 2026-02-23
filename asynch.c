/*
 * File: asynch.c
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: February 21, 2026
 *
 * Description:
 * This file implements the functions for handling asynchronous events, such as user input.
 *
 * File Status: 
 * Currently, the handleQuitToMenu function is not implemented, because
 * the menu screen is not implemented.
 *
 * The handle2p and handleRetry functions are currently the same as handle1p
 * because there is no 2 player mode or game over screen implemented.
*/

#include "asynch.h"
#include "model.h"

void handleJump(Model *model) {
    model->bird.jump_ticks_remaining = 3;
}

void handle1p(Model *model) {
    modelInit(model);
}

/* currently the same as handle1p */
void handle2p(Model *model) {
    modelInit(model);
}

/* currently the same as handle1p */
void handleRetry(Model *model) {
    modelInit(model);
}

/*
void handleQuitToMenu(Model *model) {
    modelInit(model);
}
*/

