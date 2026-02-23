/*
 * File: cond.h
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: February 22, 2026
 *
 * Description:
 * This file defines the functions for handling conditional events, such 
 * as checking for collisions and increasing the score.
 *
 * File Status: 
 * The handleBirdCollisions function checks for all possible 
 * collisions with the bird, including the pipes, the ground, and the top of the screen.
*/

#ifndef COND_H
#define COND_H
#include "model.h"

/* checks if the bird has collided with a pipe, the ground, or the top of the screen */
void handleBirdCollision(Model *model);
/* checks if the bird has passed through a set of pipes and increases score if so */
void handleScoreIncrease(Model *model);

#endif