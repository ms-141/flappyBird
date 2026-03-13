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
 * There is 1 obvious possible optimization:
 * Only draw and clear a line of pixels for the top and bottom pipes,
 * instead of clearing and redrawing the entire rectangle.
 *
 * Even though the score gets cleared and drawn every tick, this
 * is fine because the clearing ensures that the pipes don't
 * cover the score and make it impossible to see.
 */

#include "renderer.h"

UINT16 bird_bitmap[16] = {
    0x0000, 0x0000, 0x03E0, 0x0C10,
    0x1878, 0x3088, 0x608C, 0x409E,
    0x4073, 0x4001, 0x7007, 0x1004,
    0x180C, 0x0C18, 0x07F0, 0x0000};

void render_bird(const Bird *bird, UINT8 *base)
{
    /* erase previous bird */
    clear_region((UINT32 *)base, bird->prev_y, bird->x, BIRD_HEIGHT, BIRD_WIDTH);
    /* draw bird at new position */
    plot_16bit_bitmap((UINT16 *)base, bird->y, bird->x, bird_bitmap, BIRD_HEIGHT);
}

void render_pipe(const SetOfPipes *pipes, UINT8 *base)
{
    int dx = pipes->prev_x - pipes->x;
    unsigned int bottom_pipe_y = pipes->y + PIPE_GAP_SIZE;
    unsigned int bottom_pipe_height = SCREEN_HEIGHT - bottom_pipe_y;

    if (dx > 0 && dx < PIPE_WIDTH)
    {
        /* Erase the exposed right strip and draw only the new left strip. */
        clear_region((UINT32 *)base, 0, pipes->x + PIPE_WIDTH, pipes->y, dx);
        clear_region((UINT32 *)base, bottom_pipe_y, pipes->x + PIPE_WIDTH, bottom_pipe_height, dx);

        plot_rectangle((UINT32 *)base, 0, pipes->x, pipes->y, dx);
        plot_rectangle((UINT32 *)base, bottom_pipe_y, pipes->x, bottom_pipe_height, dx);
    }
    else
    {
        /* Fallback for initialization/teleport-like moves (e.g. respawn). */
        plot_rectangle((UINT32 *)base, 0, pipes->x, pipes->y, PIPE_WIDTH);
        plot_rectangle((UINT32 *)base, bottom_pipe_y, pipes->x, bottom_pipe_height, PIPE_WIDTH);
    }
}

void render_score(Score *score, UINT8 *base)
{
    char score_str[20];

    /* erase previous score */
    clear_region((UINT32 *)base, 20, 20, 16, 75);

    sprintf(score_str, "Score: %u", score->curr_score);

    plot_string(base, 20, 20, score_str);

    score->prev_score = score->curr_score;
}

/* function that gets repeated on the game loop*/
void render(Model *model, UINT8 *base)
{
    unsigned int i;

    render_bird(&model->bird, base);

    for (i = 0; i < 3; i++)
    {
        render_pipe(&model->pipes[i], base);
    }

    if (model->score.curr_score != model->score.prev_score)
    {
        render_score(&model->score, base);
    }
}