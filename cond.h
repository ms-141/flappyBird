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
 */

#ifndef COND_H
#define COND_H
#include "model.h"
#include "bird.h"
#include "pipepair.h"
#include "score.h"

#define GROUND_HEIGHT 300
#define BIRD_COLLISION_MARGIN 3

/* checks if the bird has collided with the pipes, the ground, or the top of the screen (y = 0)
1 = collided, 0 = hasn't collided */
int checkBirdCollision(Bird *bird, SetOfPipes *pipes);
/* checks if the bird has collided with a pipe, the ground, or the top of the screen */
void handleBirdCollision(Model *model);
/* checks if the bird has passed through a set of pipes and increases score if so */
void handleScoreIncrease(Model *model);

#endif