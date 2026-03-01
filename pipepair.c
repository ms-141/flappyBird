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
    pipes->x -= PIPE_MOVE_SPEED; /* Move the pipes to the left by PIPE_MOVE_SPEED pixels */
}

void resetPipes(SetOfPipes *pipes) {
    pipes->x = PIPE_RESPAWN_X; /* Resets the horizontal position of the pipes to PIPE_RESPAWN_X */
    pipes->y = 200; /* Resets the vertical position of the bottom of the top pipe */
}

int isOffScreen(SetOfPipes *pipes, unsigned int screen_width) {
    return (pipes->x + PIPE_WIDTH) <= 0; /* returns true if the right edge of the pipes is off screen */
}
