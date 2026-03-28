/*
 * File: music.c
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: March 21, 2026
 *
 * Description:
 * This file implements functions which will utilize the psg.c library to play music in the game.
 * 
 * File Status: 
 *
*/
#include "music.h"

static UINT32 accumulated_time = 0;
static int current_note = 0;

Note song[SONG_LENGTH] = 
{
    {400, 40}, 
    {700, 30},
    {800, 30},
    {900, 20}
};

void start_music() {
    current_note = 0;

    set_volume(0, 5);
    set_tone(0, song[0].tuning);
    enable_channel(0, 1, 0);
}

void update_music(UINT32 time_elapsed)
{
    accumulated_time += time_elapsed;

    if (accumulated_time >= song[current_note].duration)
    {
        accumulated_time = 0;
        current_note++;

        if (current_note >= SONG_LENGTH)
            current_note = 0;  /* loop song */

        set_tone(0, song[current_note].tuning);
    }
}