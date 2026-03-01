/*
 * File: cond.c
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: February 22, 2026
 *
 * Description:
 * This file implements the functions for handling conditional events.
 *
 * File Status: 
 * The handleBirdCollisions functions currently passes a 
 * static 150 for the ground height.
 * 
 * Currently the checkBirdCollision is passed a static 150 for the ground height
 * by the handleBirdCollisions function.
*/

#include "cond.h"

int checkBirdCollision(Bird *bird, SetOfPipes *pipes, unsigned int ground_height) {
    if (bird->y + BIRD_HEIGHT >= ground_height) 
        return 1; /* collision with the ground */

    if (bird->y <= 0) 
        return 1; /* collision with the top of the screen */
    
    if (bird->x + BIRD_WIDTH >= pipes->x && bird->x <= pipes->x + PIPE_WIDTH) {
        if (bird->y <= pipes->y || bird->y + BIRD_HEIGHT >= pipes->y + PIPE_GAP_SIZE) {
            return 1; /* collision with the pipes */
        }
    }

    return 0; /* no collision */
}

void handleBirdCollision(Model *model) {
    unsigned int i;

    if (model->state != PLAYING)
        return;
    for (i = 0; i < 3; i++) {
        if (checkBirdCollision(&model->bird, &model->pipes[i], 150)) {
            model->state = GAME_OVER;
        }
    }
}

void handlePipeRespawn(Model *model) {
    unsigned int i;

    if (model->state != PLAYING)
        return;
    for (i = 0; i < 3; i++) {
        if (isOffScreen(&model->pipes[i], 640)) { 
            resetPipes(&model->pipes[i], 640);
        }
    }
}

void handleScoreIncrease(Model *model) {
    unsigned int i;

    if (model->state != PLAYING)
        return;
    for (i = 0; i < 3; i++) {
        if (model->pipes[i].x + PIPE_WIDTH <= model->bird.x) { /* if the bird has just passed through the pipes */
            increaseScore(&model->score);
        }
    }
}