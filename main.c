#include <stdio.h>
#include <osbind.h>
#include "model.h"
#include "asynch.h"
#include "synch.h"
#include "cond.h"
#include "raster.h"

/*
Authors: Muji Shah, Gurshan Chera, Declan McCarthy
Program Details: flappy bird game!
*/

/* Raster test functions: (others are inside test.c) */


void testClearScreen(void);
void testPlotPixel(void);
void testPlotHorizontalLine(void);
void testPlotVerticalLine(void);
void testPlotLine(void);
void testPlotRectangle(void);
void testPlotSquare(void);
void testPlotTriangle(void);
void testPlot8bitBitmap(void);
void testPlot16bitBitmap(void);
void testPlot32bitBitmap(void);
void testPlotCharacter(void);
void testPlotString(void);


int main()
{

    /* Raster test function calls: */
    /*
    testClearScreen();

    testClearRegion();

    testPlotPixel();
    
    testPlotHorizontalLine();
    
    testPlotVerticalLine();
    
    testPlotLine();
    
    testPlotRectangle();

    testPlotSquare();
    
    testPlotTriangle();
    
    testPlot8bitBitmap();

    testPlot16bitBitmap();

    testPlot32bitBitmap();

    testPlotCharacter();

    testPlotString();
    */

    /* Main game loop: */

    return 0;
}

void testClearScreen()
{
    UINT32 *base = (UINT32 *)Physbase();
    clear_screen(base);
    printf("TEST clear_screen: CHECK OUTPUT \n");
}

void testClearRegion()
{
    UINT32 *base = (UINT32 *)Physbase();
    clear_region(base, 0, 0, 100, 100);
    printf("TEST clear_region: CHECK OUTPUT \n");
}

void testPlotPixel()
{
    UINT8 *base = (UINT8 *)Physbase();
    plot_pixel(base, 200, 250);
    printf("TEST plot_pixel: CHECK OUTPUT \n");
}

void testPlotHorizontalLine()
{
    UINT32 *base = (UINT32 *)Physbase();
    plot_horizontal_line(base, 200, 0, 640);
    printf("TEST plot_horizontal_line: CHECK OUTPUT \n");
}

void testPlotVerticalLine()
{
    UINT32 *base = (UINT32 *)Physbase();
    plot_vertical_line(base, 0, 200, 400);
    printf("TEST plot_vertical_line: CHECK OUTPUT \n");
}

void testPlotLine()
{
    UINT32 *base = (UINT32 *)Physbase();
    plot_line(base, 0, 0, 400, 640);
    printf("TEST plot_line: CHECK OUTPUT \n");
}

void testPlotRectangle()
{
    UINT32 *base = (UINT32 *)Physbase();
    plot_rectangle(base, 250, 350, 150, 50);
    printf("TEST plot_rectangle: CHECK OUTPUT \n");
}

void testPlotSquare()
{
    UINT32 *base = (UINT32 *)Physbase();
    plot_square(base, 100, 200, 25);
    printf("TEST plot_square: CHECK OUTPUT \n");
}

void testPlotTriangle()
{
    UINT32 *base = (UINT32 *)Physbase();
    plot_triangle(base, -5, -5, 50, 50, 0);
    printf("TEST plot_triangle: CHECK OUTPUT \n");
}

UINT8 bitmap_8bit[8] = {
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF};

void testPlot8bitBitmap()
{
    UINT8 *base = (UINT8 *)Physbase();
    plot_8bit_bitmap(base, 100, 100, bitmap_8bit, 8);
    printf("TEST plot_8bit_bitmap: CHECK OUTPUT \n");
}

UINT16 bitmap_16bit[16] = {
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};

void testPlot16bitBitmap()
{
    UINT16 *base = (UINT16 *)Physbase();
    plot_16bit_bitmap(base, 100, 200, bitmap_16bit, 16);
    printf("TEST plot_16bit_bitmap: CHECK OUTPUT \n");
}

UINT32 bitmap_32bit[32] = {
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

void testPlot32bitBitmap()
{
    UINT32 *base = (UINT32 *)Physbase();
    plot_32bit_bitmap(base, 100, 300, bitmap_32bit, 32);
    printf("TEST plot_32bit_bitmap: CHECK OUTPUT \n");
}
