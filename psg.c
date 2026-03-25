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

void read_psg(int reg)
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