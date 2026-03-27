/*
Authors: Muji Shah, Gurshan Chera, Declan McCarthy
Program Details: flappy bird game!

File Status:
The main game loop is implemented.
*/

#include <stdio.h>
#include <stdlib.h>
#include "model.h"
#include "renderer.h"
#include "raster.h"
#include "asynch.h"
#include "synch.h"
#include "cond.h"
#include "input.h"

#define TIMER_ADDR 0x462
#define FRAME_ALIGNMENT 256

UINT32 getTime(void);
UINT8 *alignTo256(UINT8 *raw_buffer);
void renderBackground(UINT32 *base);

int main()
{
    /* Main game loop: */
    Model model;
    UINT8 *original_front = (UINT8 *)Physbase();
    UINT8 *back_buffer_raw;
    UINT8 *front_buffer = original_front;
    UINT8 *back_buffer;
    UINT8 *temp_buffer;

    UINT32 time_then, time_now, time_elapsed;

    unsigned int quit = 0;

    back_buffer_raw = (UINT8 *)malloc(BYTES_PER_SCREEN + FRAME_ALIGNMENT);
    if (back_buffer_raw == NULL)
    {
        return 1;
    }
    back_buffer = alignTo256(back_buffer_raw);

    srand((unsigned int)getTime());

    /* initializing and rendering first state */
    modelInit(&model);
    clear_screen((UINT32 *)back_buffer);
    renderBackground((UINT32 *)back_buffer);
    render(&model, back_buffer);

    /* show first rendered frame at the next vertical blank */
    Setscreen(-1L, (long)back_buffer, -1);
    Vsync(); /* wait for the flip to actually happen */

    temp_buffer = front_buffer;
    front_buffer = back_buffer;
    back_buffer = temp_buffer;

    time_then = getTime();

    /* start_music(); */

    while (!quit)
    {
        if (processInput() == 1)
        {
            char input = nextInput();

            if (input == 'q' && model.state != MENU)
            {
                handleQuitToMenu(&model);
            }
            else if (input == 'q' && model.state == MENU)
            {
                quit = 1;
            }
            else if (input == ' ' && model.state == PLAYING)
            {
                handleJump(&model);
            }
            else if (input == ' ' && model.state == GAME_OVER)
            {
                handleRetry(&model);
            }
            else if (input == '1' && model.state == MENU)
            {
                handle1p(&model);
            }
            else if (input == '2' && model.state == MENU)
            {
                handle2p(&model);
            }
            /* else the input is not accepted (ignored) */
        }

        time_now = getTime();
        time_elapsed = time_now - time_then;

        if (time_elapsed > 0)
        {
            /* synchronous events */
            handleBirdMovement(&model);
            handlePipeMovement(&model);

            /* conditional events */
            handleBirdCollision(&model);
            handlePipeRespawn(&model);
            handleScoreIncrease(&model);

            clear_screen((UINT32 *)back_buffer);
            renderBackground((UINT32 *)back_buffer);
            render(&model, back_buffer);

            /* page flip: schedule, wait for vblank, then swap pointers */
            Setscreen(-1L, (long)back_buffer, -1);
            Vsync(); /* wait for the flip to actually happen */

            temp_buffer = front_buffer;
            front_buffer = back_buffer;
            back_buffer = temp_buffer;

            time_then = time_now;
        }
    }

    Setscreen(-1L, (long)original_front, -1);
    Vsync(); /* wait for original screen to be restored */
    free(back_buffer_raw);

    return 0;
}

/*
 PURPOSE: Returns the first 256-byte aligned address at or after raw_buffer.

 INPUT: raw_buffer is the start of the allocated memory block.

 OUTPUT: A 256-byte aligned buffer address within the allocated block.

 LIMITATIONS: Assumes raw_buffer is non-NULL.
*/
UINT8 *alignTo256(UINT8 *raw_buffer)
{
    unsigned long address = (unsigned long)raw_buffer;

    while ((address % FRAME_ALIGNMENT) != 0)
    {
        address++;
    }

    return (UINT8 *)address;
}

UINT32 getTime()
{
    volatile UINT32 *timer = (volatile UINT32 *)TIMER_ADDR;
    UINT32 time;
    long old_ssp;

    old_ssp = Super(0); /* enter supervisor mode to read system variable */
    time = *timer;
    Super(old_ssp); /* exit supervisor mode */

    return time;
}

void renderBackground(UINT32 *base)
{
    plot_horizontal_line(base, GROUND_HEIGHT, 0, SCREEN_WIDTH);
}