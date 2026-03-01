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
 * Currently, the handleQuitToMenu function only sets the Gamestate to 
 * MENU, because the menu screen is not implemented.
 *
 * The handle2p is currently the same as handle1p because there is no 
 * 2 player mode implemented.
*/

#include "asynch.h"

void handleJump(Model *model) {
    if (model->state == PLAYING)
        model->bird.jump_ticks_remaining = 3;
}

void handle1p(Model *model) {
    if (model->state == MENU)
        modelInit(model);
}

/* currently the same as handle1p */
void handle2p(Model *model) {
    if (model->state == MENU)
        modelInit(model);
}

void handleRetry(Model *model) {
    if (model->state == GAME_OVER)
        modelInit(model);
}

void handleQuitToMenu(Model *model) {
    model->state = MENU;
}


