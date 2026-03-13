/*
 * File: cond.c
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: February 22, 2026
 *
 * Description:
 * This file implements the functions for handling conditional events.
 *
 * File Status:
 */

#include "cond.h"

int checkBirdCollision(Bird *bird, SetOfPipes *pipes)
{
    int bird_top = bird->y + BIRD_COLLISION_MARGIN;
    int bird_bottom = bird->y + BIRD_HEIGHT - BIRD_COLLISION_MARGIN;

    if (bird->y + BIRD_HEIGHT >= GROUND_HEIGHT)
        return 1; /* collision with the ground */

    if (bird->y <= 0)
        return 1; /* collision with the top of the screen */

    if (bird->x + BIRD_WIDTH >= pipes->x && bird->x <= pipes->x + PIPE_WIDTH)
    {
        if (bird_top < (int)pipes->y || bird_bottom > (int)(pipes->y + PIPE_GAP_SIZE))
        {
            return 1; /* collision with the pipes */
        }
    }

    return 0; /* no collision */
}

void handleBirdCollision(Model *model)
{
    unsigned int i;

    if (model->state != PLAYING)
        return;
    for (i = 0; i < 3; i++)
    {
        if (checkBirdCollision(&model->bird, &model->pipes[i]))
        {
            model->state = GAME_OVER;
        }
    }
}

void handlePipeRespawn(Model *model)
{
    unsigned int i;

    if (model->state != PLAYING)
        return;
    for (i = 0; i < 3; i++)
    {
        if (isOffScreen(&model->pipes[i], 640))
        {
            resetPipes(&model->pipes[i], 640);
        }
    }
}

void handleScoreIncrease(Model *model)
{
    unsigned int i;

    if (model->state != PLAYING)
        return;
    for (i = 0; i < 3; i++)
    {
        int pipe_right_prev = model->pipes[i].prev_x + PIPE_WIDTH;
        int pipe_right_now = model->pipes[i].x + PIPE_WIDTH;

        /* score once when the pipe's right edge crosses the bird's x-position */
        if (pipe_right_prev > (int)model->bird.x && pipe_right_now <= (int)model->bird.x)
        {
            increaseScore(&model->score);
        }
    }
}