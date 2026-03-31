/*
 * File: psg.c
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

void write_psg(int reg, UINT8 val)
{
    if (reg < 0 || reg > 15) 
        return; 

    *PSG_reg_select = reg;
    *PSG_reg_write = val;
}

UINT8 read_psg(int reg)
{
    *PSG_reg_select = reg;

    return *PSG_reg_write;
}

void set_tone(int channel, int tuning)
{
    int coarse, fine;

    if (channel < 0 || channel > 2) 
        return;

    if (tuning < 0 || tuning > 0x0FFF) 
        return;

    coarse = (tuning >> 8) & 0x0F; /* upper 4 bits = coarse tuning */
    fine = tuning & 0xFF;          /* lower 8 bits = fine tuning */

    write_psg(channel * 2, fine);       /* fine register is even-numbered for each channel */
    write_psg(channel * 2 + 1, coarse); /* coarse register is odd-numbered for each channel */
}

void set_volume(int channel, int volume)
{
    if (channel < 0 || channel > 2) 
        return;

    if (volume < 0 || volume > 31)
        return;

    write_psg(8 + channel, volume & 0x1F); /* volume registers are 8-10, and only lower 5 bits are used */
}

void enable_channel(int channel, int tone_on, int noise_on)
{
    UINT8 mixer, tone_bit, noise_bit;

    if (channel < 0 || channel > 2) 
        return; 

    if ((tone_on != 0 && tone_on != 1) ||
        (noise_on != 0 && noise_on != 1))
    return;

    mixer = read_psg(7); /* read current mixer settings */
    tone_bit = 1 << channel; 
    noise_bit = 1 << (channel + 3); 

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

    write_psg(8, 0);  
    write_psg(9, 0);  
    write_psg(10, 0); 
}

void set_noise(int tuning)
{
    if (tuning < 0 || tuning > 31)
        return; 

    write_psg(6, tuning & 0x1F); /* noise register is 6, and only lower 5 bits are used */
}

void set_envelope(int shape, unsigned int sustain)
{
    if (shape < 0 || shape > 15) 
        return; 

    write_psg(11, sustain & 0xFF); /* envelope sustain low byte */
    write_psg(12, (sustain >> 8) & 0xFF); /* envelope sustain high byte */
    write_psg(13, shape & 0x0F); /* only lower 4 bits are used */
}
