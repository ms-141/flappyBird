/*
 * File: pipepair.h
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: February 18, 2026
 *
 * Description:
 * This file defines the data structures and functions for the SetOfPipes object.
 * A pair of pipes move at a constant rate to the left and spawn with a random height.
 * When pipes move off screen, they are moved back to the right edge of the screen with a new random height.
 * The bottom of the top pipe is determined by the y value and the top of the bottom pipe
 * is determined by the y value plus the defined PIPE_GAP_SIZE.
 *
 * File Status: 
*/

#ifndef PIPEPAIR_H
#define PIPEPAIR_H

#define PIPE_WIDTH 32
#define PIPE_GAP_SIZE 100
#define PIPE_MOVE_SPEED 3

typedef struct
{
    int x; /* each set of pipes has a continuously changing horizontal pos. */
    int prev_x; /* the previous horizontal pos. of the pipes used for renderer simplification */
    unsigned int y; /* the vertical pos. of the bottom of the top pipe.
                    the pipe gap size will be added to this value to 
                    determine the vertical pos. of the top of the bottom pipe */
} SetOfPipes;

/* moves the pipes by PIPE_MOVE_SPEED pixels to the left */
void movePipes(SetOfPipes *pipes); 
/* deletes pipes that are off screen and spawns new pipes with a new height */
void resetPipes(SetOfPipes *pipes, unsigned int new_x); 
/* checks if the pipes are off screen (x < 0) 
1 = off screen, 0 = on screen */
int isOffScreen(SetOfPipes *pipes, unsigned int screen_width);

#endif