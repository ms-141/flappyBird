#ifndef COND_H
#define COND_H
#include "model.h"

/* checks if the bird has collided with a pipe, the ground, or the top of the screen */
void handle_bird_collision(Model *model);
/* checks if the bird has passed through a set of pipes and increases score if so */
void handle_score_increase(Model *model);

#endif