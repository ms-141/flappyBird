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

static volatile int mouse_x = 320;
static volatile int mouse_y = 200;
static volatile unsigned char mouse_buttons = 0;
static volatile int mouse_visible = 0;

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
int get_mouse_visible(void) { return mouse_visible; }
void set_mouse_visible(int v) { mouse_visible = v; }

void move_mouse_x(int dx)
{
    mouse_x += dx;
    if (mouse_x < 0)
        mouse_x = 0;
    if (mouse_x > 639)
        mouse_x = 639;
}

void move_mouse_y(int dy)
{
    mouse_y += dy;
    if (mouse_y < 0)
        mouse_y = 0;
    if (mouse_y > 399)
        mouse_y = 399;
}

void set_mouse_buttons(unsigned char buttons)
{
    mouse_buttons = buttons;
}