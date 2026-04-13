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
#define IKBD_VECTOR_NUM 70

/* IKBD make-code scancodes (bit 7 = 0 = key press; bit 7 = 1 = break code) */
#define SC_SPACE 0x39
#define SC_Q 0x10
#define SC_1 0x02

/* MC6850 ACIA control register values (keyboard ACIA at 0xFFFC00)
   Bits [1:0] = 10  -> divide clock by 64  (7812.5 baud)
   Bits [4:2] = 101 -> 8 data bits, no parity, 1 stop bit (8N1)
   Bits [6:5] = 00  -> RTS low, Tx interrupt disabled
   Bit  7    = 1/0 -> Rx interrupt enabled / disabled                    */
#define IKBD_CTRL_RX_ENABLED 0x96  /* full TOS default: Rx IRQ on  */
#define IKBD_CTRL_RX_DISABLED 0x16 /* polling mode   : Rx IRQ off  */

/* MC6850 ACIA status register bit masks */
#define IKBD_STATUS_RDRF 0x01 /* Receive Data Register Full */

/* MFP In-Service Register B: must clear bit 6 (ACIA) after each ISR
   so the MFP will fire the interrupt again next time */
#define MFP_ISRB ((volatile UINT8 *)0xFFFA11)
#define MFP_ACIA_ISR_CLR 0xBF /* all bits 1 except bit 6 */
typedef UINT8 SCANCODE;

typedef void (*Vector)(void);

long old_ssp;
Model model;

/* Scancode ring buffer shared between ISR (writer) and main loop (reader) */
#define IKBD_BUF_SIZE 16
volatile SCANCODE ikbd_buf[IKBD_BUF_SIZE];
volatile int ikbd_head = 0; /* ISR writes here  */
volatile int ikbd_tail = 0; /* main loop reads here */

volatile UINT16 render_request = 0;

static Vector old_vbl_isr = 0;
static int vbl_installed = 0;
static Vector old_IKBD_isr = 0;
static int IKBD_installed = 0;

volatile UINT8 *const IKBD_control = (UINT8 *)0xFFFC00;
volatile const UINT8 *const IKBD_status = (const UINT8 *)0xFFFC00;
volatile const SCANCODE *const IKBD_RDR = (const SCANCODE *)0xFFFC02;

void vbl_isr(void);
void ikbd_isr(void);

UINT32 getTime(void);
UINT8 *alignTo256(UINT8 *raw_buffer);
void renderBackground(UINT32 *base);
void run_game(UINT8 *front_buffer, UINT8 *back_buffer);
int make_splash_screen(UINT8 *base);
Vector install_vector(int num, Vector vector);
void do_VBL_ISR(void);
void install_vbl(void);
void remove_vbl(void);
void do_IKBD_ISR(void);
void install_IKBD(void);
void remove_IKBD(void);

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

    /* Install IKBD ISR then load splash screen */
    install_IKBD();
    choice = make_splash_screen(back_buffer);
    if (choice == 1)
    {
        run_game(front_buffer, back_buffer);
    }
    remove_IKBD();

    set_video_base((UINT16 *)original_front);
    Vsync(); /* wait for original screen to be restored */
    free(back_buffer_raw);

    stop_sound();

    return 0;
}

/* runs the main game loop */
void run_game(UINT8 *front_buffer, UINT8 *back_buffer)
{
    UINT8 *temp_buffer;
    UINT32 time_then, time_now, time_elapsed;
    unsigned int quit = 0;
    int choice;

    modelInit(&model);

    clear_screen((UINT32 *)back_buffer);
    renderBackground((UINT32 *)back_buffer);
    render(&model, back_buffer);

    set_video_base((UINT16 *)back_buffer);
    Vsync();

    temp_buffer = front_buffer;
    front_buffer = back_buffer;
    back_buffer = temp_buffer;

    stop_sound();
    start_music();
    time_then = getTime();

    while (!quit)
    {
        Vsync();

        time_now = getTime();
        time_elapsed = time_now - time_then;
        time_then = time_now;

        while (ikbd_tail != ikbd_head)
        {
            SCANCODE scancode = ikbd_buf[ikbd_tail];
            ikbd_tail = (ikbd_tail + 1) % IKBD_BUF_SIZE;

            if (!(scancode & 0x80)) /* only handle make codes (key press) */
            {
                if (scancode == SC_Q)
                {
                    model.state = GAME_OVER;
                }
                else if (scancode == SC_SPACE && model.state == PLAYING)
                {
                    handleJump(&model);
                }
            }
        }

        update_music(time_elapsed);
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

                stop_sound();
                start_music();
                time_then = getTime();
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
    SCANCODE scancode;

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

    start_menu_music();

    while (1)
    {
        time_now = getTime();
        time_elapsed = time_now - time_then;

        update_menu_music(time_elapsed);

        time_then = time_now;

        while (ikbd_tail != ikbd_head)
        {
            scancode = ikbd_buf[ikbd_tail];
            ikbd_tail = (ikbd_tail + 1) % IKBD_BUF_SIZE;
            if (scancode == SC_1)
                return 1;
            if (scancode == SC_Q)
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

void do_IKBD_ISR(void)
{
    int next = (ikbd_head + 1) % IKBD_BUF_SIZE;
    if (next != ikbd_tail) /* drop if buffer full */
        ikbd_buf[ikbd_head] = *IKBD_RDR;
    else
        (void)*IKBD_RDR; /* must still read RDR to clear RDRF */
    ikbd_head = next;

    /* Clear MFP in-service bit so future interrupts can fire */
    *MFP_ISRB = MFP_ACIA_ISR_CLR;
}

void install_IKBD(void)
{
    if (IKBD_installed)
        return;

    /* 0xFFFC00 is in the protected I/O region: must be in supervisor mode */
    old_ssp = Super(0);
    /* Write full 8-bit value: ÷64, 8N1, Rx IRQ enabled, Tx IRQ disabled */
    *IKBD_control = IKBD_CTRL_RX_ENABLED;
    Super(old_ssp);

    old_IKBD_isr = install_vector(IKBD_VECTOR_NUM, ikbd_isr);
    ikbd_head = ikbd_tail = 0; /* flush buffer */
    IKBD_installed = 1;
}

void remove_IKBD(void)
{
    if (!IKBD_installed)
        return;

    /* Disable Rx interrupt before restoring old vector (write full 8-bit value) */
    old_ssp = Super(0);
    *IKBD_control = IKBD_CTRL_RX_DISABLED;
    Super(old_ssp);

    install_vector(IKBD_VECTOR_NUM, old_IKBD_isr);

    /* Restore TOS default: Rx interrupt enabled */
    old_ssp = Super(0);
    *IKBD_control = IKBD_CTRL_RX_ENABLED;
    Super(old_ssp);

    ikbd_head = ikbd_tail = 0;
    IKBD_installed = 0;
}