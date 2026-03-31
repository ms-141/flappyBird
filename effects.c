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

#include <osbind.h>
#include "effects.h"

void play_jump_effect()
{
    long old_ssp = Super(0);

    set_volume(2, 16); /* enable envelope (bit 4 = 1) */
    set_tone(2, 500);    

    /* (shape of sound, duration) */
    set_envelope(0x09, 1000); /* short envelope */

    enable_channel(2, 1, 0); 

    Super(old_ssp);
}

void play_game_over_effect()
{
    long old_ssp = Super(0);

    set_volume(2, 16);     
    set_tone(2, 2000);      
    set_noise(5);

    set_envelope(0x09, 7000);  /* long envelope */

    enable_channel(2, 1, 1); 

    Super(old_ssp);
}