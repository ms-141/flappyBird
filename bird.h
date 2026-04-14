/*
 * File: bird.h
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: February 18, 2026
 *
 * Description:
 * This file defines the data structures and functions for the Bird object.
 * The Bird has a fixed horizontal position and a variable vertical position
 * that changes based on user input and constant falling.
 *
 * File Status:
 */

#ifndef BIRD_H
#define BIRD_H

#define BIRD_VERT_START_POS 200
#define BIRD_X_POS 50
#define BIRD_JUMP_DELTA -6 /* bird moves up by this amount each jump tick */
#define BIRD_FALL_DELTA 1  /* bird falls by this amount each 70 Hz update tick */
#define BIRD_WIDTH 16
#define BIRD_HEIGHT 16

typedef struct
{
    unsigned int x;                    /* bird's horizontal pos. is fixed */
    int y;                             /* the birds vertical pos. changes */
    int prev_y;                        /* the bird's previous vertical pos. used for renderer simplification */
    unsigned int jump_ticks_remaining; /* the number of ticks remaining in the current jump */
} Bird;

/* initializes the bird's position at the start of the game */
void spawnBird(Bird *bird);
/* makes the bird jump upwards by subtracting from its y pos. */
void birdJump(Bird *bird);
/* makes the bird fall downwards by adding to its y pos. */
void birdFalling(Bird *bird);

#endif