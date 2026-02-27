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

void plot_pixel(UINT8 *base, int row, int col) {
    unsigned int row_offset;
    if (col >= 0 && col < SCREEN_WIDTH && 
        row >= 0 && row < SCREEN_HEIGHT)
        {
        row_offset = (row << 6) + (row << 4);   /* row * 80 */
        *(base + row_offset + (col >> 3)) |= 1 << (7 - (col & 7));
        }
}