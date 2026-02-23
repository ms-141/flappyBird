/*
 * File: score.h
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: February 19, 2026
 *
 * Description:
 * This file defines the data structures and functions for the Score object.
 * The score is incremented each time the bird moves past a pipe.
 * The high score is updated if the current score exceeds it.
 *
 * File Status: 
*/

#ifndef SCORE_H
#define SCORE_H

typedef struct
{
	unsigned int curr_score; /* current score for the current round */
	unsigned int high_score; /* highest score out of all of the rounds */
} Score;

/* increments the current score by 1*/
increaseScore(Score *score);

#endif