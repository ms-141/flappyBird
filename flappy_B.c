/*
Authors: Muji Shah, Gurshan Chera, Declan McCarthy
Program Details: flappy bird game!

File Status:
The main game loop is implemented, but the functions
for processing events are not yet implemented.
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

UINT32 getTime(void);
void renderBackground(UINT32 *base);

int main()
{
    /* Main game loop: */
    Model model;
    UINT8 *base = (UINT8 *)Physbase();

    UINT32 time_then, time_now, time_elapsed;

    unsigned int quit = 0;
    unsigned int clear_the_screen = 0; /* 0 = false, 1 = true */

    srand((unsigned int)getTime());

    /* initializing and rendering first state */
    modelInit(&model);
    clear_screen((UINT32 *)base);
    renderBackground((UINT32 *)base);
    render(&model, base);

    time_then = getTime();

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
                clear_the_screen = 1;
                handleRetry(&model);
            }
            else if (input == '1' && model.state == MENU)
            {
                clear_the_screen = 1;
                handle1p(&model);
            }
            else if (input == '2' && model.state == MENU)
            {
                clear_the_screen = 1;
                handle2p(&model);
            }
            /* else the input is not accepted (ignored) */
        }

        time_now = getTime();
        time_elapsed = time_now - time_then;

        if (time_elapsed > 0)
        {
            if (clear_the_screen)
            {
                clear_screen((UINT32 *)base);
                renderBackground((UINT32 *)base);
                clear_the_screen = 0;
            }
            /* synchronous events */
            handleBirdMovement(&model);
            handlePipeMovement(&model);

            /* conditional events */
            handleBirdCollision(&model);
            handlePipeRespawn(&model);
            handleScoreIncrease(&model);

            render(&model, base);

            time_then = time_now;
        }
    }

    return 0;
}

UINT32 getTime()
{
    volatile UINT32 *timer = (volatile UINT32 *)TIMER_ADDR;
    UINT32 time;
    long old_ssp;

    old_ssp = Super(0); /* enter privileged mode */
    time = *timer;      /* read timer */
    Super(old_ssp);     /* exit privileged mode */

    return time;
}

void renderBackground(UINT32 *base)
{
    plot_horizontal_line(base, GROUND_HEIGHT, 0, SCREEN_WIDTH);
}