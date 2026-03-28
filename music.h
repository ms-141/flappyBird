/*
 * File: music.h
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: March 21, 2026
 *
 * Description:
 * This file defines the functions for playing music in the game.
 * 
 * File Status: 
 *
*/

#ifndef MUSIC_H
#define MUSIC_H

#include "psg.h"

/* Begins the playing of the song by loading the data for the first note into the PSG. */
void start_music();

/* Advances to the next note of the song if necessary, as determined by the amount of
time elapsed since the previous call. */
void update_music(UINT32 time_elapsed);

#endif