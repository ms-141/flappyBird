/*
 * File: effects.c
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: March 29, 2026
 *
 * Description:
 * This file implements the functions for playing sound effects in the game.
 * 
 * File Status: 
*/

#include "effects.h"

void play_jump_effect()
{
    set_volume(2, 16); /* enable envelope (bit 4 = 1) */
    set_tone(2, 800);    

    /* (shape of sound, duration) */
    set_envelope(0x0A, 1000); /* short envelope */

    enable_channel(2, 1, 1); 
}

void play_game_over_effect()
{
    set_volume(2, 16);     
    set_tone(2, 800);       

    set_envelope(0x0A, 5000);  /* long envelope */

    enable_channel(2, 1, 0); 
}