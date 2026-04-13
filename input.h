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

#include <osbind.h>

int processInput(); /* Returns whether pending input needs to be processed */
char nextInput();   /* Returns the next input to be processed */

int get_mouse_x(void);         /* Returns current absolute mouse X position */
int get_mouse_y(void);         /* Returns current absolute mouse Y position */
int get_mouse_left(void);      /* Returns 1 if left mouse button is pressed */
void set_mouse_visible(int v); /* Show (1) or hide (0) the mouse pointer */

#endif