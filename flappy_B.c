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
#include "music.h"

#define TIMER_ADDR 0x462
#define FRAME_ALIGNMENT 256

UINT32 getTime(void);
UINT8 *alignTo256(UINT8 *raw_buffer);
void renderBackground(UINT32 *base);
void run_game(UINT8 *front_buffer, UINT8 *back_buffer);
int make_splash_screen(UINT8 *base);

int main()
{
    UINT8 *original_front = (UINT8 *)Physbase();
    UINT8 *back_buffer_raw;
    UINT8 *front_buffer = original_front;
    UINT8 *back_buffer;
    int choice;

    back_buffer_raw = (UINT8 *)malloc(BYTES_PER_SCREEN + FRAME_ALIGNMENT);
    if (back_buffer_raw == NULL)
    {
        return 1;
    }
    back_buffer = alignTo256(back_buffer_raw);

    srand((unsigned int)getTime());

    /* Load Splash Screen  */
    choice = make_splash_screen(back_buffer);
    if (choice == 1)
    {
        run_game(front_buffer, back_buffer);
    }

    Setscreen(-1L, (long)original_front, -1);
    Vsync(); /* wait for original screen to be restored */
    free(back_buffer_raw);

    return 0;
}

/* runs the main game loop */
void run_game(UINT8 *front_buffer, UINT8 *back_buffer)
{
    Model model;
    UINT8 *temp_buffer;
    UINT32 time_then, time_now, time_elapsed;
    unsigned int quit = 0;

    modelInit(&model);

    clear_screen((UINT32 *)back_buffer);
    renderBackground((UINT32 *)back_buffer);
    render(&model, back_buffer);

    Setscreen(-1L, (long)back_buffer, -1);
    Vsync();

    temp_buffer = front_buffer;
    front_buffer = back_buffer;
    back_buffer = temp_buffer;

    time_then = getTime();

    old_ssp = Super(0);
    start_music();
    Super(old_ssp);

    while (!quit)
    {
        if (processInput() == 1)
        {
            char input = nextInput();

            if (input == 'q')
            {
                model.state = GAME_OVER;
            }
            else if (input == ' ' && model.state == PLAYING)
            {
                handleJump(&model);
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

            old_ssp = Super(0);
            update_music(time_elapsed);
            Super(old_ssp);

            time_then = time_now;

            /* Show splash screen at game over  */
            if (model.state == GAME_OVER)
            {
                int choice = make_splash_screen(back_buffer);
                if (choice == 1)
                    modelReset(&model);
                else
                    quit = 1;
            }
        }
    }
}

int make_splash_screen(UINT8 *base)
{
    UINT32 time_then, time_now, time_elapsed;

    /* render a rectangle, everything within might have to be cleared, two options: 1p and quit */
    /* clear region for splash screen */
    /* clear only the splash screen region, preserving the background */
    clear_region((UINT32 *)base, 125, 170, 150, 300);

    /* make rectangle (4 lines) */
    plot_horizontal_line((UINT32 *)base, 125, 170, 300);
    plot_horizontal_line((UINT32 *)base, 275, 170, 300);
    plot_vertical_line((UINT32 *)base, 125, 170, 150);
    plot_vertical_line((UINT32 *)base, 125, 470, 150);

    /* title */
    plot_string((UINT8 *)base, 140, 285, "FLAPPY BIRD");

    /* write '1 - One Player' and 'Q - Quit Game'*/
    plot_string((UINT8 *)base, 175, 270, "1 - One Player");
    plot_string((UINT8 *)base, 205, 270, "Q - Quit Game");

    /* plot_horizontal_line(UINT32 *base, int row, int col, UINT16 length) */
    plot_horizontal_line((UINT32 *)base, 170, 260, 135);
    plot_horizontal_line((UINT32 *)base, 195, 260, 135);

    plot_horizontal_line((UINT32 *)base, 200, 260, 135);
    plot_horizontal_line((UINT32 *)base, 225, 260, 135);

    plot_vertical_line((UINT32 *)base, 170, 260, 25);
    plot_vertical_line((UINT32 *)base, 170, 395, 25);

    plot_vertical_line((UINT32 *)base, 200, 260, 25);
    plot_vertical_line((UINT32 *)base, 200, 395, 25);

    /* splash screen is created. display it, then wait for user input */
    Setscreen(-1L, (long)base, -1);
    Vsync();

    time_then = getTime();

    old_ssp = Super(0);
    start_menu_music();
    Super(old_ssp);

    while (1)
    {
        time_now = getTime();
        time_elapsed = time_now - time_then;

        old_ssp = Super(0);
        update_menu_music(time_elapsed);
        Super(old_ssp);

        time_then = time_now;

        if (processInput() == 1)
        {
            char input = nextInput();
            if (input == '1')
                return 1;
            if (input == 'q')
                return 0;
        }
    }
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