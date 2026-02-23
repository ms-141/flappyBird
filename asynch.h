/*
 * File: asynch.h
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: February 21, 2026
 *
 * Description:
 * This file defines the functions for handling asynchronous events, such as user input.
 * This file handles the user input for jumping, starting a 1 player game, starting a 2 player game, 
 * retrying after game over, and quitting to the menu after game over.
 *
 * File Status: 
 * Currently, the user inputs are not actually grabbed, but they 
 * should work as intended when called.
*/

#ifndef ASYNCH_H
#define ASYNCH_H
#include "model.h"

/* handles user depressing spacebar in game */
void handleJump(Model *model);
/* handles user depressing '1' in menu */
void handle1p(Model *model);
/* handles user depressing '2' in menu */
void handle2p(Model *model);
/* handles user depressing spacebar in game over screen */
void handleRetry(Model *model);
/* handles user depressing 'q' in game over screen */
void handleQuitToMenu(Model *model);

#endif