/*
 * File: rndertst.c
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: February 28, 2026
 *
 * Description:
 * Test driver for the renderer.
 *
 * File Status: 
 * Since clock ticks are used yet, things like the bird jumping,
 * or the pipes moving are done instantly (instead of over time).
*/

#include <stdio.h>
#include <osbind.h>
#include "model.h"
#include "synch.h"
#include "raster.h"
#include "renderer.h"

int main()
{
    UINT8 *base = (UINT8 *)Physbase();
    Model myModel;

    int i;

    clear_screen((UINT32 *)base);
    printf("=== Rendering Tests ===\n");

    /* Test 1: Initial state */
    modelInit(&myModel);
    render(&myModel, base);
    while (!Cconis())
    ; /* wait for key press */
    Cnecin(); /* clear key press */
    clear_screen((UINT32 *)base);
    
    /* Test 2: Bird jumped */
    modelInit(&myModel);
    myModel.bird.jump_ticks_remaining = 3;
    for (i = 0; i < 3; i++)
    {
        handleBirdMovement(&myModel);
    }
    render(&myModel, base);
    while (!Cconis())
    ; 
    Cnecin(); 
    clear_screen((UINT32 *)base);
   
    /* Test 3: Pipes moved */
    modelInit(&myModel);
    myModel.pipes[0].x = 100;
    myModel.pipes[1].x = 250;
    myModel.pipes[2].x = 400;
    render(&myModel, base);
    while (!Cconis())
    ; 
    Cnecin(); 
    clear_screen((UINT32 *)base);

    /* Test 4: Score increased */
    modelInit(&myModel);
    increaseScore(&myModel.score);
    render(&myModel, base);
    while (!Cconis())
    ;
    Cnecin();
    clear_screen((UINT32 *)base);

    return 0;
}