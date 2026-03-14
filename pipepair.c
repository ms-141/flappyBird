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
 */

#include "pipepair.h"
#include <stdlib.h>

static unsigned int randomPipeY(void)
{
    unsigned int range = PIPE_MAX_Y - PIPE_MIN_Y + 1;
    return PIPE_MIN_Y + ((unsigned int)rand() % range);
}

void movePipes(SetOfPipes *pipes)
{
    pipes->prev_x = pipes->x;    /* update the previous horizontal position before changing it */
    pipes->x -= PIPE_MOVE_SPEED; /* Move the pipes to the left by PIPE_MOVE_SPEED pixels */
}

void resetPipes(SetOfPipes *pipes, unsigned int new_x)
{
    pipes->prev_x = pipes->x; /* update the previous horizontal position */
    pipes->x = new_x;         /* Resets the horizontal position of the pipes to new_x */

    pipes->y = randomPipeY(); /* respawn with random gap height */
}

int isOffScreen(SetOfPipes *pipes, unsigned int screen_width)
{
    return (pipes->x + PIPE_WIDTH) <= 0; /* returns true if the right edge of the pipes is off screen */
}
