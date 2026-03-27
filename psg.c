/*
 * File: psg.h
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: March 21, 2026
 *
 * Description:
 * This file implements the PSG chip functions which will be used to play music and sound effects in the game.
 * 
 * File Status: 
 *
*/

#include "psg.h"

volatile char *PSG_reg_select = 0xFF8800;
volatile char *PSG_reg_write  = 0xFF8802;
long old_ssp = Super(0);

void write_psg(int reg, UINT8 val)
{
    *PSG_reg_select = reg;
    *PSG_reg_write = val;
}

UINT8 read_psg(int reg)
{
    *PSG_reg_select = reg;
    return *PSG_reg_write;  /* return contents of selected register */
}

void set_tone(int channel, int tuning)
{
    int coarse = (tuning >> 8) & 0x0F; /* upper 4 bits = coarse tuning */
    int fine = tuning & 0xFF;           /* lower 8 bits = fine tuning */

    write_psg(channel * 2, fine);       /* fine register is even-numbered for each channel*/
    write_psg(channel * 2 + 1, coarse); /* coarse register is odd-numbered for each channel */
}

void set_volume(int channel, int volume)
{
    write_psg(8 + channel, volume & 0x0F); /* volume registers are 8-10, and only lower 4 bits are used */
}

void enable_channel(int channel, int tone_on, int noise_on)
{
    int mixer = read_psg(7); /* read current mixer settings */
    int tone_bit = 1 << channel; /* bit for this channel's tone */
    int noise_bit = 1 << (channel + 3); /* bit for this channel's noise */

    if (tone_on)
        mixer &= ~tone_bit; /* clear bit to enable tone (0 = tone enabled) */
    else
        mixer |= tone_bit;  /* set bit to disable tone (1 = tone disabled)*/

    if (noise_on)
        mixer &= ~noise_bit; /* clear bit to enable noise */
    else
        mixer |= noise_bit;  /* set bit to disable noise */

    write_psg(7, mixer); /* write updated mixer settings back to PSG */
}

void stop_sound() {
    write_psg(7, 0x3F); /* set all bits in mixer to disable all channels */
}

void set_noise(int tuning)
{
    write_psg(6, tuning & 0x1F); /* noise register is 6, and only lower 5 bits are used */
}
