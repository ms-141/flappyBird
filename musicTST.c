/*
 * File: musicTST.c
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: March 28, 2026
 *
 * Description:
 * Test driver for the music.
 *
 * File Status: 
*/

#include "music.h"
#include <osbind.h>

int main() 
{
    long old_ssp = Super(0);
    UINT32 last = 0;

    start_music();

    while (!Cconis())
    {
        UINT32 now = *(volatile UINT32*)0x462;  /* 70Hz clock */

        if (now != last)
        {
            update_music(now - last);
            last = now;
        }
    }
    Cnecin();
    stop_sound();

    Super(old_ssp);
    return 0;
}