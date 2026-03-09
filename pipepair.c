/*
 * File: pipepair.c
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: February 18, 2026
 *
 * Description:
 * This file implements the functions for the SetOfPipes object.
 *
 * File Status: 
 * Currently the height of the pipes is not random, it is set to a static 200
*/

#include "pipepair.h"

void movePipes(SetOfPipes *pipes) {
    pipes->prev_x = pipes->x; /* update the previous horizontal position before changing it */
    pipes->x -= PIPE_MOVE_SPEED; /* Move the pipes to the left by PIPE_MOVE_SPEED pixels */
}

void resetPipes(SetOfPipes *pipes, unsigned int new_x) {
    pipes->x = new_x; /* Resets the horizontal position of the pipes to new_x */
    pipes->prev_x = pipes->x; /* update the previous horizontal position */
    pipes->y = 200; /* Resets the vertical position of the bottom of the top pipe */
}

int isOffScreen(SetOfPipes *pipes, unsigned int screen_width) {
    return (pipes->x + PIPE_WIDTH) <= 0; /* returns true if the right edge of the pipes is off screen */
}
