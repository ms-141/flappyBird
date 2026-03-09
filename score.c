/*
 * File: score.c
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: February 19, 2026
 *
 * Description:
 * This file implements the functions for the Score object.
 *
 * File Status: 
*/

#include "score.h"

void increaseScore(Score *score) {
    score->prev_score = score->curr_score; /* update the previous score before changing it */
    score->curr_score += 1;
    
    if (score->curr_score > score->high_score) {
        score->high_score = score->curr_score; /* update high score if current score exceeds it */
    }
}