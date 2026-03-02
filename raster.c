/*
 * File: raster.c
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: February 23, 2026
 *
 * Description:
 * This file implements the functions for the Raster routines.
 *
 * File Status:
 */

#include "raster.h"
#include <linea.h>

void clear_screen(UINT32 *base)
{
    UINT32 *loc = base;
    unsigned int i;

    for (i = 0; i < LONGS_PER_SCREEN; i++)
        *(loc++) = 0;
}

void clear_region(UINT32 *base, int row, int col, UINT16 length, UINT16 width)
{
    int r, c; /* r = row, c = column */

    int start_row = row;
    int start_col = col;
    int end_row = row + length;
    int end_col = col + width;

    /* completely off-screen (return) */
    if (end_row <= 0 || end_col <= 0 ||
        start_row >= SCREEN_HEIGHT ||
        start_col >= SCREEN_WIDTH)
        return;

    /* top is clipped */
    if (start_row < 0)
        start_row = 0;
    /* left is clipped */
    if (start_col < 0)
        start_col = 0;
    /* bottom is clipped */
    if (end_row > SCREEN_HEIGHT)
        end_row = SCREEN_HEIGHT;
    /* right is clipped */
    if (end_col > SCREEN_WIDTH)
        end_col = SCREEN_WIDTH;

    for (r = start_row; r < end_row; r++)
    {
        unsigned int row_offset = (r << 6) + (r << 4); /* r * 80 */

        for (c = start_col; c < end_col; c++)
        {
            UINT8 *byte = (UINT8 *)base + row_offset + (c >> 3);

            *byte &= ~(1 << (7 - (c & 7))); /* clear pixel by masking the byte, only pixel bit is cleared */
        }
    }
}

void plot_pixel(UINT8 *base, int row, int col)
{
    unsigned int row_offset;
    if (col >= 0 && col < SCREEN_WIDTH &&
        row >= 0 && row < SCREEN_HEIGHT)
    {
        row_offset = (row << 6) + (row << 4); /* row * 80 */
        *(base + row_offset + (col >> 3)) |= 1 << (7 - (col & 7));
    }
}

void plot_horizontal_line(UINT32 *base, int row, int col, UINT16 length)
{
    UINT32 SOLID = 0xFFFFFFFF;
    UINT32 p1, p2;
    int x, i, end;
    int shift_F, shift_B;
    UINT32 mask;
    UINT32 *place;

    /* line completely off-screen (return) */
    if (row < 0 || row >= SCREEN_HEIGHT)
    {
        return;
    }
    /* left is clipped */
    if (col < 0)
    {
        col = 0;
    }
    /* right is clipped */
    if (col >= SCREEN_WIDTH)
    {
        col = SCREEN_WIDTH - 1;
    }

    place = base + row * 20;
    x = col >> 5; /* divide by 32 */
    end = ((col + length) - 1) >> 5;
    shift_F = col & 31;
    shift_B = (32 - 1) - (((col + length) - 1) & 31);

    if (x == end)
    {
        p1 = SOLID >> shift_F;
        p2 = SOLID << shift_B;
        mask = (p1) & (p2);
        *(place + x) |= mask;
    }
    else
    {
        p1 = SOLID >> shift_F;
        p2 = SOLID << shift_B;
        *(place + x) |= p1;

        for (i = x + 1; i < end; i++)
        {
            *(place + i) = SOLID;
        }
        *(place + end) |= p2;
    }
}

void plot_vertical_line(UINT32 *base, int row, int col, UINT16 length)
{
    int temp;
    UINT32 pattern;
    UINT32 *screen_long;
    int end = (row + length) - 1;

    /* line on the screen */
    if (col >= 0 && col < SCREEN_WIDTH)
    {
        /* top is clipped */
        if (row < 0)
        {
            row = 0;
        }
        /* bottom is clipped */
        if (end > 399)
        {
            end = SCREEN_HEIGHT - 1;
        }
        pattern = 1 << (31 - (col & 31));
        screen_long = base + row * 20 + (col >> 5);
        for (; row <= end; row++)
        {
            *screen_long |= pattern;
            screen_long = screen_long + 20;
        }
    }
}

void plot_line(UINT32 *base, int start_row, int start_col, int end_row, int end_col)
{
    int delta_col;
    int delta_row;
    int step_col;
    int step_row;
    int error;
    int current_col;
    int current_row;
    UINT8 *base8;

    base8 = (UINT8 *)base;
    current_col = start_col;
    current_row = start_row;

    delta_col = end_col - start_col;
    if (delta_col < 0)
        delta_col = -delta_col;

    delta_row = end_row - start_row;
    if (delta_row < 0)
        delta_row = -delta_row;

    step_col = (start_col < end_col) ? 1 : -1;
    step_row = (start_row < end_row) ? 1 : -1;

    error = delta_col - delta_row;

    while (1)
    {
        plot_pixel(base8, current_row, current_col);

        if (current_col == end_col && current_row == end_row)
            break;

        if ((error << 1) > -delta_row)
        {
            error -= delta_row;
            current_col += step_col;
        }

        if ((error << 1) < delta_col)
        {
            error += delta_col;
            current_row += step_row;
        }
    }
}

void plot_triangle(UINT32 *base, int row, int col, UINT16 triangle_base, UINT16 height, UINT8 direction)
{
    int vertex_row_a;
    int vertex_col_a;
    int vertex_row_b;
    int vertex_col_b;
    int vertex_row_c;
    int vertex_col_c;

    if (triangle_base == 0 || height == 0)
        return;

    if (direction == 0)
    {
        vertex_row_a = row;
        vertex_col_a = col;
        vertex_row_b = row;
        vertex_col_b = col + triangle_base - 1;
        vertex_row_c = row + height - 1;
        vertex_col_c = col;
    }
    else if (direction == 1)
    {
        vertex_row_a = row;
        vertex_col_a = col;
        vertex_row_b = row;
        vertex_col_b = col - triangle_base + 1;
        vertex_row_c = row + height - 1;
        vertex_col_c = col;
    }
    else if (direction == 2)
    {
        vertex_row_a = row;
        vertex_col_a = col;
        vertex_row_b = row;
        vertex_col_b = col + triangle_base - 1;
        vertex_row_c = row - height + 1;
        vertex_col_c = col;
    }
    else if (direction == 3)
    {
        vertex_row_a = row;
        vertex_col_a = col;
        vertex_row_b = row;
        vertex_col_b = col - triangle_base + 1;
        vertex_row_c = row - height + 1;
        vertex_col_c = col;
    }
    else
    {
        return;
    }

    plot_line(base, vertex_row_a, vertex_col_a, vertex_row_b, vertex_col_b);
    plot_line(base, vertex_row_a, vertex_col_a, vertex_row_c, vertex_col_c);
    plot_line(base, vertex_row_b, vertex_col_b, vertex_row_c, vertex_col_c);
}

void plot_rectangle(UINT32 *base, int row, int col, UINT16 length, UINT16 width)
{
    int r;
    int c;
    UINT8 *base8 = (UINT8 *)base;

    if (length == 0 || width == 0)
        return;

    for (r = 0; r < length; r++)
    {
        for (c = 0; c < width; c++)
        {
            plot_pixel(base8, row + r, col + c);
        }
    }
}

void plot_square(UINT32 *base, int row, int col, UINT16 side)
{
    plot_rectangle(base, row, col, side, side);
}

void plot_8bit_bitmap(UINT8 *base, int row, int col, const UINT8 *bitmap, UINT16 height)
{
    int r, c; /* r = row, c = column */

    int start_row = row;
    int start_col = col;
    int end_row = row + height;
    int end_col = col + 8; /* fixed width */

    /* completely off-screen (return) */
    if (end_row <= 0 || end_col <= 0 ||
        start_row >= SCREEN_HEIGHT ||
        start_col >= SCREEN_WIDTH)
        return;

    /* top is clipped */
    if (start_row < 0)
    {
        bitmap += -start_row;
        start_row = 0;
    }
    /* left is clipped */
    if (start_col < 0)
        start_col = 0;
    /* bottom is clipped */
    if (end_row > SCREEN_HEIGHT)
        end_row = SCREEN_HEIGHT;
    /* right is clipped */
    if (end_col > SCREEN_WIDTH)
        end_col = SCREEN_WIDTH;

    for (r = start_row; r < end_row; r++)
    {
        int row_index = r - start_row; /* row index in the bitmap */
        UINT8 data = bitmap[row_index];

        unsigned int row_offset = (r << 6) + (r << 4);

        for (c = start_col; c < end_col; c++)
        {
            int bit_index = c - col;

            /* if the current bit in the bitmap is set, draw it */
            if (data & (1 << (7 - bit_index)))
            {
                UINT8 *byte = base + row_offset + (c >> 3);

                *byte |= 1 << (7 - (c & 7));
            }
        }
    }
}

void plot_16bit_bitmap(UINT16 *base, int row, int col, const UINT16 *bitmap, UINT16 height)
{
    int r, c; /* r = row, c = column */

    int start_row = row;
    int start_col = col;
    int end_row = row + height;
    int end_col = col + 16; /* fixed width for 16-bit bitmap */

    UINT8 *base8 = (UINT8 *)base;

    /* completely off-screen (return) */
    if (end_row <= 0 || end_col <= 0 ||
        start_row >= SCREEN_HEIGHT ||
        start_col >= SCREEN_WIDTH)
        return;

    /* top is clipped */
    if (start_row < 0)
    {
        bitmap += -start_row;
        start_row = 0;
    }
    /* left is clipped */
    if (start_col < 0)
        start_col = 0;
    /* bottom is clipped */
    if (end_row > SCREEN_HEIGHT)
        end_row = SCREEN_HEIGHT;
    /* right is clipped */
    if (end_col > SCREEN_WIDTH)
        end_col = SCREEN_WIDTH;
    for (r = start_row; r < end_row; r++)
    {
        int row_index = r - start_row; /* row index in the bitmap */
        UINT16 data = bitmap[row_index];

        unsigned int row_offset = (r << 6) + (r << 4);

        for (c = start_col; c < end_col; c++)
        {
            int bit_index = c - col;

            /* if the current bit in the bitmap is set, draw it */
            if (data & (1U << (15 - bit_index)))
            {
                UINT8 *byte = base8 + row_offset + (c >> 3);

                *byte |= 1 << (7 - (c & 7));
            }
        }
    }
}

void plot_32bit_bitmap(UINT32 *base, int row, int col, const UINT32 *bitmap, UINT16 height)
{
    int r, c;

    int start_row = row;
    int start_col = col;
    int end_row = row + height;
    int end_col = col + 32; /* fixed width for 32-bit bitmap */

    UINT8 *base8 = (UINT8 *)base;

    /* completely off-screen (return) */
    if (end_row <= 0 || end_col <= 0 ||
        start_row >= SCREEN_HEIGHT ||
        start_col >= SCREEN_WIDTH)
        return;

    /* top is clipped */
    if (start_row < 0)
    {
        bitmap += -start_row;
        start_row = 0;
    }
    /* left is clipped */
    if (start_col < 0)
        start_col = 0;
    /* bottom is clipped */
    if (end_row > SCREEN_HEIGHT)
        end_row = SCREEN_HEIGHT;
    /* right is clipped */
    if (end_col > SCREEN_WIDTH)
        end_col = SCREEN_WIDTH;

    for (r = start_row; r < end_row; r++)
    {
        int row_index = r - start_row; /* row index in the bitmap */
        UINT32 data = bitmap[row_index];

        unsigned int row_offset = (r << 6) + (r << 4);

        for (c = start_col; c < end_col; c++)
        {
            int bit_index = c - col;

            /* if the current bit in the bitmap is set, draw it */
            if (data & (1UL << (31 - bit_index)))
            {
                UINT8 *byte = base8 + row_offset + (c >> 3);
                *byte |= 1 << (7 - (c & 7));
            }
        }
    }
}

void plot_character(UINT8 *base, int row, int col, char ch)
{
    UINT8 glyph[16];
    UINT8 *font_table;
    unsigned int i;

    linea0();
    font_table = (UINT8 *)V_FNT_AD;

    for (i = 0; i < 16; i++)
    {
        glyph[i] = *(font_table + (unsigned char)ch + (i << 8));
    }

    plot_8bit_bitmap(base, row, col, glyph, 16);
}

void plot_string(UINT8 *base, int row, int col, char *ch)
{
    unsigned int i;

    if (ch == 0)
        return;

    for (i = 0; ch[i] != '\0'; i++)
    {
        plot_character(base, row, col + (i * 8), ch[i]);
    }
}