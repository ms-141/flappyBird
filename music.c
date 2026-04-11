/*
 * File: music.c
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: March 21, 2026
 *
 * Description:
 * This file implements functions which will utilize the psg.c library to play music in the game.
 *
 * File Status: created a new function to enable the music channel A in case it gets disabled.
 *
 */
#include "music.h"

static UINT32 accumulated_time = 0;
static int current_note = 0;

/* to re enalbe music */
static void keep_music_channel_alive(void)
{
    set_volume(0, 5);
    enable_channel(0, 1, 0);
}

static void update_track(const Note *track, UINT32 time_elapsed)
{
    accumulated_time += time_elapsed;

    keep_music_channel_alive();

    while (accumulated_time >= (UINT32)track[current_note].duration)
    {
        accumulated_time -= (UINT32)track[current_note].duration;
        current_note++;

        if (current_note >= SONG_LENGTH)
            current_note = 0; /* loop song */

        set_tone(0, track[current_note].tuning);
    }
}

Note song[SONG_LENGTH] =
    {
        {400, 40},
        {700, 30},
        {800, 30},
        {900, 20}};

Note menu_song[SONG_LENGTH] =
    {
        {508, 40},
        {285, 30},
        {320, 30},
        {254, 20}};

void start_music()
{
    current_note = 0;
    accumulated_time = 0;

    set_tone(0, song[0].tuning);
    keep_music_channel_alive();
}

void update_music(UINT32 time_elapsed)
{
    update_track(song, time_elapsed);
}

void start_menu_music()
{
    current_note = 0;
    accumulated_time = 0;

    set_tone(0, menu_song[0].tuning);
    keep_music_channel_alive();
}

void update_menu_music(UINT32 time_elapsed)
{
    update_track(menu_song, time_elapsed);
}