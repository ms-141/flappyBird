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
    /* the width of the region to be cleared when the pipes move */
    int dx = PIPE_MOVE_SPEED;
    /* the x-coordinate of the region to be cleared when the pipes move*/
    unsigned int clear_x = pipes->x + PIPE_WIDTH;

    unsigned int bottom_pipe_y = pipes->y + PIPE_GAP_SIZE;
    unsigned int bottom_pipe_height = SCREEN_HEIGHT - bottom_pipe_y;

    /* the y-coordinate of the region to be cleared when the pipes move. 
    This is to make sure that after the pipe height is randomized, the 
    pixels on the left of the screen are correctly cleared */
    unsigned int pipe_y_to_clear;
    /* bottom pipe */
    unsigned int b_pipe_y_to_clear;
    unsigned int b_pipe_height_to_clear;

    if (clear_x >= SCREEN_WIDTH)
    {
        clear_x -= SCREEN_WIDTH; /* wrap around to the left edge of the screen */
        pipe_y_to_clear = pipes->prev_y;
        b_pipe_y_to_clear = pipes->prev_y + PIPE_GAP_SIZE;
    }
    else
    {
        pipe_y_to_clear = pipes->y;
        b_pipe_y_to_clear = pipes->y + PIPE_GAP_SIZE;
    }
    b_pipe_height_to_clear = SCREEN_HEIGHT - b_pipe_y_to_clear;

    /* erase the exposed right strip and draw only the new left strip. */
    clear_region((UINT32 *)base, 0, clear_x, pipe_y_to_clear, dx);
    clear_region((UINT32 *)base, b_pipe_y_to_clear, clear_x, b_pipe_height_to_clear, dx);

    plot_rectangle((UINT32 *)base, 0, pipes->x, pipes->y, dx);
    plot_rectangle((UINT32 *)base, bottom_pipe_y, pipes->x, bottom_pipe_height, dx);

    /* redraw the ground where the pipes where cleared */
    plot_horizontal_line((UINT32 *)base, GROUND_HEIGHT, clear_x, dx);
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

    render_score(&model->score, base);
}