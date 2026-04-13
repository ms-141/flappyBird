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

/* Mouse state is owned by the IKBD ISR in flappy_B.c */
extern volatile int mouse_x;
extern volatile int mouse_y;
extern volatile unsigned char mouse_buttons;
extern volatile int mouse_visible;

int processInput()
{
    if (Cconis())
    {
        return 1;
    }
    return 0;
}

char nextInput()
{
    return (char)Cnecin();
}

int get_mouse_x(void) { return mouse_x; }
int get_mouse_y(void) { return mouse_y; }
int get_mouse_left(void) { return (mouse_buttons & 0x02) ? 1 : 0; }
void set_mouse_visible(int v) { mouse_visible = v; }