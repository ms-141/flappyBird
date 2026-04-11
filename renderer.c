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

/* Fast fill for exactly 32px-wide pipe slices when fully on-screen. */
static void fill_pipe_rect_fast(UINT32 *base, int row, int col, int height)
{
    int r;
    int word_index = col >> 5;
    int shift = col & 31;

    if (height <= 0)
        return;

    if (shift == 0)
    {
        for (r = row; r < row + height; r++)
        {
            UINT32 *row_ptr = base + (r * 20);
            row_ptr[word_index] = 0xFFFFFFFFUL;
        }
    }
    else
    {
        UINT32 left_mask = 0xFFFFFFFFUL >> shift;
        UINT32 right_mask = 0xFFFFFFFFUL << (32 - shift);

        for (r = row; r < row + height; r++)
        {
            UINT32 *row_ptr = base + (r * 20);
            row_ptr[word_index] |= left_mask;
            row_ptr[word_index + 1] |= right_mask;
        }
    }
}

void render_bird(const Bird *bird, UINT8 *base)
{
    /* full redraw mode: only draw bird at current position */
    plot_16bit_bitmap((UINT16 *)base, bird->y, bird->x, bird_bitmap, BIRD_HEIGHT);
}

void render_pipe(const SetOfPipes *pipes, UINT8 *base)
{
    /* full redraw mode: draw full pipe rectangles every frame */
    UINT32 *base32 = (UINT32 *)base;
    unsigned int bottom_pipe_y = pipes->y + PIPE_GAP_SIZE;
    unsigned int bottom_pipe_height = SCREEN_HEIGHT - bottom_pipe_y;

    if (pipes->x >= 0 && pipes->x <= (SCREEN_WIDTH - PIPE_WIDTH))
    {
        fill_pipe_rect_fast(base32, 0, pipes->x, pipes->y);
        fill_pipe_rect_fast(base32, bottom_pipe_y, pipes->x, bottom_pipe_height);
    }
    else
    {
        /* fallback handles clipping when a pipe is partially off-screen */
        plot_rectangle(base32, 0, pipes->x, pipes->y, PIPE_WIDTH);
        plot_rectangle(base32, bottom_pipe_y, pipes->x, bottom_pipe_height, PIPE_WIDTH);
    }
}

void render_score(Score *score, UINT8 *base)
{
    static char score_str[20];

    /* Rebuild string only when value changes; still draw every frame. */
    if (score->curr_score != score->prev_score)
    {
        sprintf(score_str, "Score: %u", score->curr_score);
        score->prev_score = score->curr_score;
    }

    plot_string(base, 20, 20, score_str);
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