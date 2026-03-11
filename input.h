/*
 * File: input.h
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: March 2, 2026
 *
 * Description:
 * Defines the input routines needed for the game.
 *
 * File Status:
 *
*/
#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include "model.h"

bool processInput(); /* Returns whether pending input needs to be processed */
char nextInput(); /* Returns the next input to be proccessed */

#endif