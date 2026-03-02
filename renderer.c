/*
 * File: renderer.c
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: February 28, 2026
 *
 * Description:
 * This file implements the functions for the Renderer routines.
 *
 * File Status: 
 * There is no bird_bitmap currently defined.
 * 
 * render_pipe will not work until the plot_rectangle is implemented
 * 
 * render_score will not work until the plot_string is implemented
*/

#include "renderer.h"

UINT16 bird_bitmap [16] = {
	0x0000, 0x0000, 0x03E0, 0x0C10,
	0x1878, 0x3088, 0x608C, 0x409E,
	0x4073, 0x4001, 0x7007, 0x1004,
	0x180C, 0x0C18, 0x07F0, 0x0000
};

void render_bird(const Bird *bird, UINT8 *base) 
{
    plot_16bit_bitmap((UINT16 *)base, bird->y, bird->x, bird_bitmap, BIRD_HEIGHT);
}

void render_pipe(const SetOfPipes *pipes, UINT8 *base) 
{
    unsigned int bottom_pipe_y = pipes->y + PIPE_GAP_SIZE;
    unsigned int bottom_pipe_height = SCREEN_HEIGHT - bottom_pipe_y;

    /* Top pipe */
    plot_rectangle((UINT32 *)base, 0, pipes->x, pipes->y, PIPE_WIDTH);

    /* Bottom pipe */
    plot_rectangle((UINT32 *)base, bottom_pipe_y, pipes->x, bottom_pipe_height, PIPE_WIDTH);
}

void render_score(const Score *score, UINT8 *base) 
{
    char score_str[20];

    sprintf(score_str, "Score: %u", score->curr_score);

    plot_string(base, 20, 20, score_str);
}

void render(const Model *model, UINT8 *base) 
{
    unsigned int i;

    render_bird(&model->bird, base);

    for (i = 0; i < 3; i++)
    {
        render_pipe(&model->pipes[i], base);
    }

    render_score(&model->score, base);
}

