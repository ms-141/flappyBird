/*
 * File: input.c
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: March 2, 2026
 *
 * Description:
 * This file wraps underlying, O/S-specific code for performing keyboard and mouse input.
 *
 * File Status:
 *
*/

#include "input.h"

void inputJump(Bird *bird) {
    char jumpKey = ' '; 
    char key = Cconin(); /* get the key that the user pressed */
    if (key == jumpKey) {   /* if the key is the spacebar, make the bird jump */
         birdJump(bird);
    }



}