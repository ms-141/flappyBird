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
            /* processAsyncEvents(&model, key) */
        }

        timeNow = get_time();
        timeElapsed = timeNow - timeThen;

        if (timeElapsed > 0)
        {
            /* processSyncEvents(&model) */

            /* processCondEvents(&model) */

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