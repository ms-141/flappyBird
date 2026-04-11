/*
Authors: Muji Shah, Gurshan Chera, Declan McCarthy
Program Details: flappy bird game!

File Status:
The main game loop is implemented.
*/

#include <stdio.h>
#include <stdlib.h>
#include "model.h"
#include "renderer.h"
#include "raster.h"
#include "asynch.h"
#include "synch.h"
#include "cond.h"
#include "input.h"
#include "music.h"
#include "effects.h"

#define TIMER_ADDR 0x462
#define FRAME_ALIGNMENT 256
#define VBL_VECTOR_NUM 28

typedef void (*Vector)(void);

long old_ssp;
Model model;
volatile UINT16 render_request = 0;

static Vector old_vbl_isr = 0;
static int vbl_installed = 0;

void vbl_isr(void);

UINT32 getTime(void);
UINT8 *alignTo256(UINT8 *raw_buffer);
void renderBackground(UINT32 *base);
void run_game(UINT8 *front_buffer, UINT8 *back_buffer);
int make_splash_screen(UINT8 *base);
Vector install_vector(int num, Vector vector);
void do_VBL_ISR(void);
void install_vbl(void);
void remove_vbl(void);

int main()
{
    UINT8 *original_front = (UINT8 *)get_video_base();
    UINT8 *back_buffer_raw;
    UINT8 *front_buffer = original_front;
    UINT8 *back_buffer;
    int choice;

    back_buffer_raw = (UINT8 *)malloc(BYTES_PER_SCREEN + FRAME_ALIGNMENT);
    if (back_buffer_raw == NULL)
    {
        return 1;
    }
    back_buffer = alignTo256(back_buffer_raw);

    srand((unsigned int)getTime());

    /* initialize splash background before first display */
    clear_screen((UINT32 *)back_buffer);
    renderBackground((UINT32 *)back_buffer);

    /* Load Splash Screen  */
    choice = make_splash_screen(back_buffer);
    if (choice == 1)
    {
        run_game(front_buffer, back_buffer);
    }

    set_video_base((UINT16 *)original_front);
    Vsync(); /* wait for original screen to be restored */
    free(back_buffer_raw);

    old_ssp = Super(0);
    stop_sound();
    Super(old_ssp);

    return 0;
}

/* runs the main game loop */
void run_game(UINT8 *front_buffer, UINT8 *back_buffer)
{
    UINT8 *temp_buffer;
    unsigned int quit = 0;
    int choice;
    char input;

    modelInit(&model);

    clear_screen((UINT32 *)back_buffer);
    renderBackground((UINT32 *)back_buffer);
    render(&model, back_buffer);

    set_video_base((UINT16 *)back_buffer);
    Vsync();

    temp_buffer = front_buffer;
    front_buffer = back_buffer;
    back_buffer = temp_buffer;

    old_ssp = Super(0);
    stop_sound();
    start_music();
    Super(old_ssp);

    while (!quit)
    {
        Vsync();

        if (processInput() == 1)
        {
            input = nextInput();

            if (input == 'q')
            {
                model.state = GAME_OVER;
            }
            else if (input == ' ' && model.state == PLAYING)
            {
                handleJump(&model);
            }
            /* else the input is not accepted (ignored) */
        }

        update_music(1);
        handleBirdMovement(&model);
        handlePipeMovement(&model);
        handleBirdCollision(&model);
        handlePipeRespawn(&model);
        handleScoreIncrease(&model);

        clear_screen((UINT32 *)back_buffer);
        renderBackground((UINT32 *)back_buffer);
        render(&model, back_buffer);

        /* page flip: hardware switches on the next VBL */
        set_video_base((UINT16 *)back_buffer);

        temp_buffer = front_buffer;
        front_buffer = back_buffer;
        back_buffer = temp_buffer;

        /* Show splash screen at game over  */
        if (model.state == GAME_OVER)
        {
            choice = make_splash_screen(back_buffer);
            if (choice == 1)
            {
                modelReset(&model);

                clear_screen((UINT32 *)back_buffer);
                renderBackground((UINT32 *)back_buffer);
                render(&model, back_buffer);

                set_video_base((UINT16 *)back_buffer);
                Vsync();

                temp_buffer = front_buffer;
                front_buffer = back_buffer;
                back_buffer = temp_buffer;

                old_ssp = Super(0);
                stop_sound();
                start_music();
                Super(old_ssp);
            }
            else
            {
                quit = 1;
            }
        }
    }
}

int make_splash_screen(UINT8 *base)
{
    UINT32 time_then, time_now, time_elapsed;
    char input;

    /* render a rectangle, everything within might have to be cleared, two options: 1p and quit */
    /* clear region for splash screen */
    /* clear only the splash screen region, preserving the background */
    clear_region((UINT32 *)base, 125, 170, 150, 300);

    /* make rectangle (4 lines) */
    plot_horizontal_line((UINT32 *)base, 125, 170, 300);
    plot_horizontal_line((UINT32 *)base, 275, 170, 300);
    plot_vertical_line((UINT32 *)base, 125, 170, 150);
    plot_vertical_line((UINT32 *)base, 125, 470, 150);

    /* title */
    plot_string((UINT8 *)base, 140, 285, "FLAPPY BIRD");

    /* write '1 - One Player' and 'Q - Quit Game'*/
    plot_string((UINT8 *)base, 175, 270, "1 - One Player");
    plot_string((UINT8 *)base, 205, 270, "Q - Quit Game");

    /* plot_horizontal_line(UINT32 *base, int row, int col, UINT16 length) */
    plot_horizontal_line((UINT32 *)base, 170, 260, 135);
    plot_horizontal_line((UINT32 *)base, 195, 260, 135);

    plot_horizontal_line((UINT32 *)base, 200, 260, 135);
    plot_horizontal_line((UINT32 *)base, 225, 260, 135);

    plot_vertical_line((UINT32 *)base, 170, 260, 25);
    plot_vertical_line((UINT32 *)base, 170, 395, 25);

    plot_vertical_line((UINT32 *)base, 200, 260, 25);
    plot_vertical_line((UINT32 *)base, 200, 395, 25);

    /* splash screen is created. display it, then wait for user input */
    set_video_base((UINT16 *)base);
    Vsync();

    time_then = getTime();

    old_ssp = Super(0);
    start_menu_music();
    Super(old_ssp);

    while (1)
    {
        time_now = getTime();
        time_elapsed = time_now - time_then;

        old_ssp = Super(0);
        update_menu_music(time_elapsed);
        Super(old_ssp);

        time_then = time_now;

        if (processInput() == 1)
        {
            input = nextInput();
            if (input == '1')
                return 1;
            if (input == 'q')
                return 0;
        }
    }
}

/*
 PURPOSE: Returns the first 256-byte aligned address at or after raw_buffer.

 INPUT: raw_buffer is the start of the allocated memory block.

 OUTPUT: A 256-byte aligned buffer address within the allocated block.

 LIMITATIONS: Assumes raw_buffer is non-NULL.
*/
UINT8 *alignTo256(UINT8 *raw_buffer)
{
    unsigned long address = (unsigned long)raw_buffer;

    while ((address % FRAME_ALIGNMENT) != 0)
    {
        address++;
    }

    return (UINT8 *)address;
}

UINT32 getTime()
{
    volatile UINT32 *timer = (volatile UINT32 *)TIMER_ADDR;
    UINT32 time;

    old_ssp = Super(0); /* enter supervisor mode to read system variable */
    time = *timer;
    Super(old_ssp); /* exit supervisor mode */

    return time;
}

void renderBackground(UINT32 *base)
{
    plot_horizontal_line(base, GROUND_HEIGHT, 0, SCREEN_WIDTH);
}

void do_VBL_ISR(void)
{
    /* Keep ISR minimal: schedule one game tick. */
    render_request = 1;
}

void install_vbl(void)
{
    if (vbl_installed)
        return;

    old_vbl_isr = install_vector(VBL_VECTOR_NUM, vbl_isr);

    render_request = 0;
    vbl_installed = 1;
}

void remove_vbl(void)
{
    if (!vbl_installed)
        return;

    install_vector(VBL_VECTOR_NUM, old_vbl_isr);

    render_request = 0;
    vbl_installed = 0;
}

Vector install_vector(int num, Vector vector)
{
    Vector orig;
    Vector *vectp = (Vector *)((long)num << 2);

    old_ssp = Super(0);
    orig = *vectp;
    *vectp = vector;
    Super(old_ssp);

    return orig;
}