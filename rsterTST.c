/*
 * File: rsderTST.c
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: March 4, 2026
 *
 * Description:
 * Test driver for the raster functions.
 *
 * File Status: 
*/

#include <stdio.h>
#include <osbind.h>
#include "raster.h"

void testClearScreen(void);
void testClearRegion(void);
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

    return 0;
}

void testClearScreen()
{
    UINT32 *base = (UINT32 *)Physbase();
    clear_screen(base);
    printf("TEST clear_screen: \n");
    while (!Cconis())
    ;
    Cnecin();
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

void testClearRegion()
{
    UINT32 *base = (UINT32 *)Physbase();
    clear_screen((UINT32 *)base);
    printf("TEST clear_region: Click to see \n");
    plot_32bit_bitmap(base, 200, 320, bitmap_32bit, 32);
    while (!Cconis())
    ;
    Cnecin();
    clear_region(base, 205, 325, 22, 22);
    while (!Cconis())
    ;
    Cnecin();
}

void testPlotPixel()
{
    UINT8 *base = (UINT8 *)Physbase();
    clear_screen((UINT32 *)base);
    printf("TEST plot_pixel: Middle of screen \n");
    plot_pixel(base, 200, 320);
    while (!Cconis())
    ;
    Cnecin();
}

void testPlotHorizontalLine()
{
    UINT32 *base = (UINT32 *)Physbase();
    clear_screen((UINT32 *)base);
    printf("TEST plot_horizontal_line: Left clipped, middle, right clipped \n");
    plot_horizontal_line(base, 100, -50, 100);
    plot_horizontal_line(base, 200, 270, 100);
    plot_horizontal_line(base, 300, 590, 100);
    while (!Cconis())
    ;
    Cnecin();
}

void testPlotVerticalLine()
{
    UINT32 *base = (UINT32 *)Physbase();
    clear_screen((UINT32 *)base);
    printf("TEST plot_vertical_line: Top clipped, middle, bottom clipped \n");
    plot_vertical_line(base, -50, 200, 100);
    plot_vertical_line(base, 150, 300, 100);
    plot_vertical_line(base, 350, 400, 100);
    while (!Cconis())
    ;
    Cnecin();
}

void testPlotLine()
{
    UINT32 *base = (UINT32 *)Physbase();
    clear_screen((UINT32 *)base);
    printf("TEST plot_line: Top-left to bottom-right, bottom-left to top-right \n");
    plot_line(base, 0, 0, 400, 640);
    plot_line(base, 400, 0, 0, 640);
    while (!Cconis())
    ;
    Cnecin();
}

void testPlotRectangle()
{
    UINT32 *base = (UINT32 *)Physbase();
    clear_screen((UINT32 *)base);
    printf("TEST plot_rectangle: Top-left, middle, bottom-right \n");
    plot_rectangle(base, -50, -50, 200, 100);
    plot_rectangle(base, 100, 350, 200, 100);
    plot_rectangle(base, 250, 590, 200, 100);
    while (!Cconis())
    ;
    Cnecin();
}

void testPlotSquare()
{
    UINT32 *base = (UINT32 *)Physbase();
    clear_screen((UINT32 *)base);
    printf("TEST plot_square: Top-left, middle, bottom-right \n");
    plot_square(base, -50, -50, 150);
    plot_square(base, 200, 245, 150);
    plot_square(base, 300, 540, 150);
    while (!Cconis())
    ;
    Cnecin();
}

void testPlotTriangle()
{
    UINT32 *base = (UINT32 *)Physbase();
    clear_screen((UINT32 *)base);
    printf("TEST plot_triangle: Top-left, 4 rotations, bottom-right \n");
    plot_triangle(base, -5, -5, 50, 50, 0);
    plot_triangle(base, 200, 320, 50, 50, 0);
    plot_triangle(base, 200, 320, 50, 50, 1);
    plot_triangle(base, 200, 320, 50, 50, 2);
    plot_triangle(base, 200, 320, 50, 50, 3);
    plot_triangle(base, 405, 645, 50, 50, 3);
    while (!Cconis())
    ;
    Cnecin();
}

UINT8 bitmap_8bit[8] = {
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF};

void testPlot8bitBitmap()
{
    UINT8 *base = (UINT8 *)Physbase();
    clear_screen((UINT32 *)base);
    printf("TEST plot_8bit_bitmap: Middle of screen \n");
    plot_8bit_bitmap(base, 200, 320, bitmap_8bit, 8);
    while (!Cconis())
    ;
    Cnecin();
}

UINT16 bird_test_bitmap[16] = {
    0xFFFF, 0xFFFF, 0x03E0, 0x0C10,
    0x1878, 0x3088, 0x608C, 0x409E,
    0x4073, 0x4001, 0x7007, 0x1004,
    0x180C, 0x0C18, 0x07F0, 0xFFFF};

void testPlot16bitBitmap()
{
    UINT16 *base = (UINT16 *)Physbase();
    clear_screen((UINT32 *)base);
    printf("TEST plot_16bit_bitmap: Top-left, middle, bottom-right \n");
    plot_16bit_bitmap(base, -5, -5, bird_test_bitmap, 16);
    plot_16bit_bitmap(base, 200, 320, bird_test_bitmap, 16);
    plot_16bit_bitmap(base, 389, 629, bird_test_bitmap, 16);
    while (!Cconis())
    ;
    Cnecin();
}

void testPlot32bitBitmap()
{
    UINT32 *base = (UINT32 *)Physbase();
    clear_screen((UINT32 *)base);
    printf("TEST plot_32bit_bitmap: Top-left, middle, bottom-right \n");
    plot_32bit_bitmap(base, -10, -10, bitmap_32bit, 32);
    plot_32bit_bitmap(base, 200, 320, bitmap_32bit, 32);
    plot_32bit_bitmap(base, 384, 624, bitmap_32bit, 32);
    while (!Cconis())
    ;
    Cnecin();
}

void testPlotCharacter()
{
    UINT8 *base = (UINT8 *)Physbase();
    clear_screen((UINT32 *)base);
    printf("TEST plot_character: Middle of screen ('A') \n");
    plot_character(base, 200, 320, 'A');
    while (!Cconis())
    ;
    Cnecin();
}

void testPlotString()
{
    UINT8 *base = (UINT8 *)Physbase();
    clear_screen((UINT32 *)base);
    printf("TEST plot_string: Middle of screen ('Hello, World!') \n");
    plot_string(base, 150, 320, "Hello, World!");
    while (!Cconis())
    ;
    Cnecin();
}