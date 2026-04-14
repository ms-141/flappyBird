/*
 * File: fxTST.c
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: March 29, 2026
 *
 * Description:
 * Test driver for the effects.
 *
 * File Status:
 */

#include <osbind.h>
#include "effects.h"
#include "psg.h"

int main()
{
    long i;
    long old_ssp;

    play_jump_effect();

    for (i = 0; i < 500000; i++);

    play_game_over_effect();

    for (i = 0; i < 500000; i++);

    old_ssp = Super(0);
    stop_sound();
    Super(old_ssp);

    return 0;
}