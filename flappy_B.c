/*
Authors: Muji Shah, Gurshan Chera, Declan McCarthy
Program Details: flappy bird game!

File Status:
The main game loop is implemented, but the functions 
for processing events are not yet implemented.
*/

#include <stdio.h>
#include <stdbool.h>
#include "model.h"
#include "renderer.h"
#include "asynch.h"
#include "synch.h"
#include "cond.h"
#include "input.h"

#define TIMER_ADDR 0x462

UINT32 get_time(void);

int main()
{
    /* Main game loop: */
    Model model;
    UINT8 *base = (UINT8 *)Physbase();

    UINT32 timeThen, timeNow, timeElapsed;

    bool quit = false;

    /* initializing and rendering first state */
    modelInit(&model);
    render(&model, base);

    timeThen = get_time();

    while (!quit)
    {
        if (processInput())
        {
            char input = nextInput();

            if (input == 'q' && model.state != MENU)
            {
                handleQuitToMenu(&model);
            }
            else if (input == 'q' && model.state == MENU)
            {
                quit = true;
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

        timeNow = get_time();
        timeElapsed = timeNow - timeThen;

        if (timeElapsed > 0)
        {
            /* synchronous events */
            handleBirdMovement(&model);
            handlePipeMovement(&model);

            /* conditional events */
            handleBirdCollision(&model);
            handlePipeRespawn(&model);
            handleScoreIncrease(&model);

            render(&model, base);

            timeThen = timeNow;
        }
    }

    return 0;
}

UINT32 get_time()
{
    volatile UINT32 *timer = (volatile UINT32 *)TIMER_ADDR;
    UINT32 time;
    long old_ssp;

    old_ssp = Super(0); /* enter privileged mode */
    time = *timer; /* read timer */
    Super(old_ssp); /* exit privileged mode */

    return time;
}