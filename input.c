/*
 * File: input.c
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: March 2, 2026
 *
 * Description:
 * This file wraps underlying, O/S-specific code for performing keyboard input.
 *
 * File Status:
 *
*/

#include "input.h"

int processInput() {
    if (Cconis()) {
        return 1;
    }
    return 0;
}

char nextInput() {
    return (char)Cnecin();
}