/*
 * File: synch.c
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: February 21, 2026
 *
 * Description:
 * This file implements the functions for handling the synchronous events.
 * For the pipes, as they move to the left each tick, they are checked to see if they
 * are off screen, in which case they are moved back to the right edge of the screen
 * with a new random height.
 *
 * File Status: 
 * The height of the pipes is currently not random, it is set to a static 200.
*/

#include "synch.h"

void handleBirdMovement(Model *model) {
    Bird *bird = &model->bird;

    if (bird->jump_ticks_remaining > 0) {
        birdJump(bird); 
        bird->jump_ticks_remaining -= 1; /* decrease the number of jump ticks remaining */
    }
    else {
        birdFalling(bird); /* if the bird is not currently jumping, make it fall */
    }
}

void handlePipeMovement(Model *model) {
    unsigned int i;

    for (i = 0; i < 3; i++) {
        movePipes(&model->pipes[i], 3); /* move the pipes to the left by 3 pixels */
    }
}

void handlePipeRespawn(Model *model) {
    unsigned int i;

    for (i = 0; i < 3; i++) {
        if (isOffScreen(&model->pipes[i], 400)) { 
            resetPipes(&model->pipes[i], 400, 200); /* respawn the pipes at the right
                                                    edge of the screen, height isn't random currently */
        }
    }
}