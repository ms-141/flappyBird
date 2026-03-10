#include <stdio.h>
#include <osbind.h>
#include "model.h"
#include "asynch.h"
#include "synch.h"
#include "cond.h"
#include "raster.h"
#include "input.h"

/*
Authors: Muji Shah, Gurshan Chera, Declan McCarthy
Program Details: flappy bird game!
*/

int main()
{
    /* Main game loop: */
    Model model;
    UINT8 *base = Physbase();
    bool quit = false;

    modelInit(&model);
    render(&model, base);


    return 0;
}
