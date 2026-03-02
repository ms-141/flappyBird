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

/* Function prototypes (some of these functions have been moved to test.c)*/

/*
void printModelState(void);
void printTestHeader(void);
*/

void testModelInit(void);
void testClearScreen(void);
void testPlotPixel(void);
void testPlotHorizontalLine(void);
void testPlotVerticalLine();

/*
void testAsynchJump(void);
void testSynchBirdMovement(void);
void testSynchPipeMovement(void);
void testCondCollisionTop(void);
void testCondCollisionGround(void);
void testCondCollisionPipe(void);
void testCondScoreIncrease(void);
*/

int main()
{

    /* Test function calls: */
    /*
    testModelInit();

    testClearScreen();

    testClearRegion();

    testPlotPixel();

    testPlotHorizontalLine();
    
    testPlot8bitBitmap();
   
    testPlot16bitBitmap();

    testPlot32bitBitmap();
    */

    /* Main game loop: */

    return 0;
}

void testModelInit()
{
    int i;
    int passed = 1;

    /* Make a model to test */
    Model myModel;
    modelInit(&myModel);

    /* Printing fields of test model */
    printf("Bird Position: x = %u, y = %d, jumpticks = %u \n", myModel.bird.x, myModel.bird.y, myModel.bird.jump_ticks_remaining);

    /* Pipes test */
    for (i = 0; i < 3; i++)
    {
        printf("Pipe[%d] position: x = %d, y = %u \n", i, myModel.pipes[i].x, myModel.pipes[i].y);
    }
    /* testing score */
    printf("Score: curr = %u high = %u \n", myModel.score.curr_score, myModel.score.high_score);

    if (myModel.bird.x != BIRD_X_POS)
        passed = 0;
    if (myModel.bird.y != BIRD_VERT_START_POS)
        passed = 0;

    if (myModel.pipes[0].x != 200 || myModel.pipes[0].y != 200)
        passed = 0;
    if (myModel.pipes[1].x != 350 || myModel.pipes[1].y != 200)
        passed = 0;
    if (myModel.pipes[2].x != 500 || myModel.pipes[2].y != 200)
        passed = 0;

    if (myModel.score.curr_score != 0)
        passed = 0;

    if (passed)
    {
        printf("TEST modelInit: PASS \n");
    }
    else
    {
        printf("TEST modelInit: FAIL \n");
    }
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
    plot_horizontal_line(base, 50, 0, 640);
    printf("TEST plot_horizontal_line: CHECK OUTPUT \n");
}

UINT8 bitmap_8bit [8] = {
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF
};

void testPlot8bitBitmap()
{
    UINT8 *base = (UINT8 *)Physbase();
    plot_8bit_bitmap(base, 100, 100, bitmap_8bit, 8);
    printf("TEST plot_8bit_bitmap: CHECK OUTPUT \n");
}

UINT16 bitmap_16bit [16] = {
	0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
	0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
	0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
	0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
};

void testPlot16bitBitmap()
{
    UINT16 *base = (UINT16 *)Physbase();
    plot_16bit_bitmap(base, 100, 200, bitmap_16bit, 16);
    printf("TEST plot_16bit_bitmap: CHECK OUTPUT \n");
}

UINT32 bitmap_32bit [32] = {
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
};

void testPlot32bitBitmap()
{
    UINT32 *base = (UINT32 *)Physbase();
    plot_32bit_bitmap(base, 100, 300, bitmap_32bit, 32);
    printf("TEST plot_32bit_bitmap: CHECK OUTPUT \n");
}