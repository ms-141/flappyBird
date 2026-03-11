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
 * The handle2p is currently the same as handle1p because there is no 
 * 2 player mode implemented.
*/

#include "asynch.h"

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

void handleRetry(Model *model) {
    modelReset(model);
}

void handleQuitToMenu(Model *model) {
    model->state = MENU;
}


