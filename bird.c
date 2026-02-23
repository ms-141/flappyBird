/*
 * File: bird.c
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: February 18, 2026
 *
 * Description:
 * This file implements the functions for the Bird object.
 *
 * File Status: 
*/

#include "bird.h"

void spawnBird(Bird *bird) {
    bird->x = BIRD_X_POS; /* set the bird's horizontal pos. to the defined constant */
    bird->y = BIRD_VERT_START_POS; /* set the bird's vertical pos. to the defined constant */
}

void birdJump(Bird *bird) {
    bird->y += BIRD_JUMP_DELTA; /* change vertical pos. by the jump delta */
}

void birdFalling(Bird *bird) {
    bird->y += BIRD_FALL_DELTA; /* change vertical pos. by the fall delta */
}