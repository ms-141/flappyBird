/*
 * File: model.c
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: February 18, 2026
 *
 * Description:
 * This file implements the functions for the Model object.
 *
 * File Status:
 */

#include "model.h"

void modelInit(Model *model)
{
    model->score.high_score = 0;

    modelReset(model);
}

void modelReset(Model *model)
{
    unsigned int i;
    unsigned int variable_distance = 0; /* the distance between each set of pipes */

    spawnBird(&model->bird);
    for (i = 0; i < 3; i++)
    {
        resetPipes(&model->pipes[i], 320 + variable_distance);
        variable_distance += 214; /* increase the distance for the next set of pipes */
    }
    model->score.curr_score = 0;
    model->score.prev_score = (unsigned int)-1; /* force score draw after reset */

    model->state = PLAYING;
}