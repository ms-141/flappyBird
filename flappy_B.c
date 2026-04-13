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

/* Saved pixel row under the cursor (XOR restore approach) */
volatile int mouse_prev_x = 320;
volatile int mouse_prev_y = 200;

/* Splash buffer pointer used by do_VBL_ISR to draw the cursor */
static UINT8 *splash_base = 0;

volatile UINT16 render_request = 0;
volatile int game_vbl_active = 0;
volatile UINT16 music_ticks_pending = 0;

static Vector old_vbl_isr = 0;
static int vbl_installed = 0;
static Vector old_IKBD_isr = 0;
static int IKBD_installed = 0;

/* Mouse packet header byte has top 6 bits = 111110, value always >= 0xF8 */
#define MOUSE_HEADER_MIN 0xF8
#define MOUSE_LEFT_BTN 0x02
#define MOUSE_RIGHT_BTN 0x01

/* ISR packet-reassembly state (0 = waiting for header, 1 = dx, 2 = dy) */
static volatile int mouse_state = 0;
static volatile UINT8 mouse_header = 0;
volatile UINT8 *const IKBD_control = (UINT8 *)0xFFFC00;
volatile const UINT8 *const IKBD_status = (const UINT8 *)0xFFFC00;
volatile const SCANCODE *const IKBD_RDR = (const SCANCODE *)0xFFFC02;

void vbl_isr(void);
void ikbd_isr(void);

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
static void xor_cursor(UINT8 *base, int row, int col);
void erase_cursor(UINT8 *base);

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

    srand(42);

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

    splash_base = NULL; /* disable cursor drawing; game doesn't use mouse */
    music_ticks_pending = 0;
    game_vbl_active = 1;
    install_vbl();

    while (!quit)
    {
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

        if (!render_request)
            continue;
        render_request = 0;

        /* keep music speed steady */
        while (music_ticks_pending > 0)
        {
            update_music(1);
            music_ticks_pending--;
        }

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
            play_game_over_effect();
            game_vbl_active = 0;
            remove_vbl();

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

                music_ticks_pending = 0;
                game_vbl_active = 1;
                install_vbl();
            }
            else
            {
                quit = 1;
            }
        }
    }

    game_vbl_active = 0;
    remove_vbl();
}

int make_splash_screen(UINT8 *base)
{
    SCANCODE scancode;
    int mx, my, left_was_down, left_now;

    /* Button regions (row_top, row_bot, col_left, col_right) */
    const int BTN1_TOP = 170, BTN1_BOT = 195, BTN1_LEFT = 260, BTN1_RIGHT = 395;
    const int BTNQ_TOP = 200, BTNQ_BOT = 225, BTNQ_LEFT = 260, BTNQ_RIGHT = 395;

    /* clear only the splash screen region, preserving the background */
    clear_region((UINT32 *)base, 125, 170, 150, 300);

    /* outer border */
    plot_horizontal_line((UINT32 *)base, 125, 170, 300);
    plot_horizontal_line((UINT32 *)base, 275, 170, 300);
    plot_vertical_line((UINT32 *)base, 125, 170, 150);
    plot_vertical_line((UINT32 *)base, 125, 470, 150);

    /* title */
    plot_string((UINT8 *)base, 140, 285, "FLAPPY BIRD");

    /* button labels */
    plot_string((UINT8 *)base, 175, 270, "1 - One Player");
    plot_string((UINT8 *)base, 205, 270, "Q - Quit Game");

    /* "1 Player" button outline */
    plot_horizontal_line((UINT32 *)base, BTN1_TOP, BTN1_LEFT, BTN1_RIGHT - BTN1_LEFT);
    plot_horizontal_line((UINT32 *)base, BTN1_BOT, BTN1_LEFT, BTN1_RIGHT - BTN1_LEFT);
    plot_vertical_line((UINT32 *)base, BTN1_TOP, BTN1_LEFT, BTN1_BOT - BTN1_TOP);
    plot_vertical_line((UINT32 *)base, BTN1_TOP, BTN1_RIGHT, BTN1_BOT - BTN1_TOP);

    /* "Quit" button outline */
    plot_horizontal_line((UINT32 *)base, BTNQ_TOP, BTNQ_LEFT, BTNQ_RIGHT - BTNQ_LEFT);
    plot_horizontal_line((UINT32 *)base, BTNQ_BOT, BTNQ_LEFT, BTNQ_RIGHT - BTNQ_LEFT);
    plot_vertical_line((UINT32 *)base, BTNQ_TOP, BTNQ_LEFT, BTNQ_BOT - BTNQ_TOP);
    plot_vertical_line((UINT32 *)base, BTNQ_TOP, BTNQ_RIGHT, BTNQ_BOT - BTNQ_TOP);

    /* Display the splash screen and start the VBL + cursor */
    set_video_base((UINT16 *)base);
    Vsync();

    splash_base = base;
    mouse_prev_x = get_mouse_x();
    mouse_prev_y = get_mouse_y();
    xor_cursor(base, get_mouse_y(), get_mouse_x()); /* draw cursor before VBL starts */
    set_mouse_visible(1);
    install_vbl();

    start_menu_music();
    left_was_down = 0;

    while (1)
    {
        /* Spin until our VBL fires; Vsync() would hang since we replaced vector 28 */
        while (!render_request)
            ;
        render_request = 0;

        update_menu_music(1); /* 1 tick per VBL; getTime() is frozen since we own vector 28 */

        /* Keyboard shortcuts still work */
        while (ikbd_tail != ikbd_head)
        {
            scancode = ikbd_buf[ikbd_tail];
            ikbd_tail = (ikbd_tail + 1) % IKBD_BUF_SIZE;
            if (scancode == SC_1)
            {
                erase_cursor(base);
                remove_vbl();
                set_mouse_visible(0);
                return 1;
            }
            if (scancode == SC_Q)
            {
                erase_cursor(base);
                remove_vbl();
                set_mouse_visible(0);
                return 0;
            }
        }

        /* Mouse click detection: fire on button release inside a button */
        left_now = get_mouse_left();
        if (!left_now && left_was_down)
        {
            mx = get_mouse_x();
            my = get_mouse_y();

            if (mx >= BTN1_LEFT && mx <= BTN1_RIGHT &&
                my >= BTN1_TOP && my <= BTN1_BOT)
            {
                erase_cursor(base);
                remove_vbl();
                set_mouse_visible(0);
                return 1;
            }
            if (mx >= BTNQ_LEFT && mx <= BTNQ_RIGHT &&
                my >= BTNQ_TOP && my <= BTNQ_BOT)
            {
                erase_cursor(base);
                remove_vbl();
                set_mouse_visible(0);
                return 0;
            }
        }
        left_was_down = left_now;
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

void renderBackground(UINT32 *base)
{
    plot_horizontal_line(base, GROUND_HEIGHT, 0, SCREEN_WIDTH);
}

/* 8x8 XOR arrow cursor bitmap (MSB = leftmost pixel) */
static const UINT8 cursor_bitmap[8] = {
    0xFF, /* ######## */
    0xFE, /* ####### */
    0xFC, /* ######  */
    0xF8, /* #####   */
    0xF0, /* ####    */
    0xE0, /* ###     */
    0xC0, /* ##      */
    0x80  /* #       */
};

static void xor_cursor(UINT8 *base, int row, int col)
{
    int r;
    for (r = 0; r < 8; r++)
    {
        int byte_col = col / 8;
        int bit_shift = col % 8;
        UINT8 *addr;

        if (row + r < 0 || row + r >= SCREEN_HEIGHT)
            continue;

        addr = base + (row + r) * BYTES_PER_ROW + byte_col;

        /* XOR first byte */
        *addr ^= cursor_bitmap[r] >> bit_shift;

        /* XOR overflow into next byte if cursor straddles a byte boundary */
        if (bit_shift > 0 && byte_col + 1 < BYTES_PER_ROW)
            *(addr + 1) ^= cursor_bitmap[r] << (8 - bit_shift);
    }
}

/* Erase cursor from screen before disabling the VBL ISR */
void erase_cursor(UINT8 *base)
{
    xor_cursor(base, mouse_prev_y, mouse_prev_x);
}

void do_VBL_ISR(void)
{
    if (game_vbl_active)
    {
        music_ticks_pending++;
        handleBirdMovement(&model);
        handlePipeMovement(&model);
        handleBirdCollision(&model);
        handlePipeRespawn(&model);
        handleScoreIncrease(&model);
    }

    render_request = 1;

    if (get_mouse_visible() && splash_base)
    {
        int mouse_x = get_mouse_x();
        int mouse_y = get_mouse_y();

        /* Erase cursor at previous position, draw at current position */
        xor_cursor(splash_base, mouse_prev_y, mouse_prev_x);
        xor_cursor(splash_base, mouse_y, mouse_x);
        mouse_prev_x = mouse_x;
        mouse_prev_y = mouse_y;
    }
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
    UINT8 byte = *IKBD_RDR;

    if (mouse_state == 0)
    {
        if (byte >= MOUSE_HEADER_MIN)
        {
            /* First byte of a 3-byte mouse packet */
            mouse_header = byte;
            mouse_state = 1;
        }
        else
        {
            /* Keyboard scancode: enqueue it */
            int next = (ikbd_head + 1) % IKBD_BUF_SIZE;
            if (next != ikbd_tail)
                ikbd_buf[ikbd_head] = byte;
            ikbd_head = next;
        }
    }
    else if (mouse_state == 1)
    {
        /* Delta X (signed) */
        move_mouse_x((signed char)byte);
        mouse_state = 2;
    }
    else
    {
        /* Delta Y (signed) */
        move_mouse_y((signed char)byte);
        set_mouse_buttons(mouse_header & (MOUSE_LEFT_BTN | MOUSE_RIGHT_BTN));
        mouse_state = 0;
    }

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