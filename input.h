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

int get_mouse_x(void);                         /* Returns current absolute mouse X position */
int get_mouse_y(void);                         /* Returns current absolute mouse Y position */
int get_mouse_left(void);                      /* Returns 1 if left mouse button is pressed */
int get_mouse_visible(void);                   /* Returns 1 if the mouse pointer is visible */
void set_mouse_visible(int v);                 /* Show (1) or hide (0) the mouse pointer */
void move_mouse_x(int dx);                     /* Adds dx to the mouse X position */
void move_mouse_y(int dy);                     /* Adds dy to the mouse Y position */
void set_mouse_buttons(unsigned char buttons); /* Updates mouse button bits */

#endif