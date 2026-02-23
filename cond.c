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
*/

#include "cond.h"
#include "bird.h"
#include "set_of_pipes.h"
#include "score.h"

void handleBirdCollision(Model *model) {
    unsigned int i;

    for (i = 0; i < 3; i++) {
        if (checkBirdCollision(&model->bird, &model->pipes[i], 150)) { /* check for collision with the pipes, ground height is currently 150 */
            model->GameState = GAME_OVER; /* if the bird collides with a pipe, the game is over */
        }
    }
}

void handleScoreIncrease(Model *model) {
    unsigned int i;

    for (i = 0; i < 3; i++) {
        if (model->pipes[i].x + PIPE_WIDTH <= model->bird.x) { /* if the bird has just passed through the pipes */
            increaseScore(&model->score);
        }
    }
}