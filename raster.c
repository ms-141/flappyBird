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
    /* right is clipped */
    if (end_row > SCREEN_HEIGHT) 
        end_row = SCREEN_HEIGHT;
    /* bottom is clipped */
    if (end_col > SCREEN_WIDTH)  
        end_col = SCREEN_WIDTH;

    for (r = start_row; r < end_row; r++)
    {
        unsigned int row_offset = (r << 6) + (r << 4); /* r * 80 */

        for (c = start_col; c < end_col; c++)
        {
            UINT8 *byte = (UINT8 *)base + row_offset + (c >> 3);

            *byte &= ~(1 << (7 - (c & 7)));  /* clear pixel */
        }
    }
}

void plot_pixel(UINT8 *base, int row, int col) {
    unsigned int row_offset;
    if (col >= 0 && col < SCREEN_WIDTH && 
        row >= 0 && row < SCREEN_HEIGHT)
        {
        row_offset = (row << 6) + (row << 4);   /* row * 80 */
        *(base + row_offset + (col >> 3)) |= 1 << (7 - (col & 7));
        }
}

void plot_horizontal_line(UINT32 *base, int row, int col, UINT16 length) {
     unsigned int row_offset;
    if (col >= 0 && col < SCREEN_WIDTH && 
        row >= 0 && row < SCREEN_HEIGHT) {
            row_offset = (row << 6) + (row << 4);   /* row * 80 */
            while (length > 0 && col < SCREEN_WIDTH) {
                *(base + row_offset + (col >> 3)) |= 1 << (7 - (col & 7));
                col++;
                length--;
            }   
        }
}

void plot_vertical_line(UINT32 *base, int row, int col, UINT16 length) {
    
}