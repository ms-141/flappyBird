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