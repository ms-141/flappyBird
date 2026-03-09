/*
 * File: synch.c
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: February 21, 2026
 *
 * Description:
 * This file implements the functions for handling the synchronous events.
 *
 * File Status: 
*/

#include "synch.h"

void handleBirdMovement(Model *model) {
    Bird *bird = &model->bird;

    if (model->state != PLAYING)
        return; 
    if (bird->jump_ticks_remaining > 0) 
    {
        birdJump(bird); 
        bird->jump_ticks_remaining -= 1; /* decrease the number of jump ticks remaining */
    }
    else 
    {
        birdFalling(bird); /* if the bird is not currently jumping, make it fall */
    }
}

void handlePipeMovement(Model *model) {
    unsigned int i;

    if (model->state != PLAYING)
        return;
    for (i = 0; i < 3; i++) 
    {
        movePipes(&model->pipes[i]);
    }
}