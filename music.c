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

void start_music() {
    set_volume(0,5);
    set_tone(0, 0xD5D);
    enable_channel(0, 1, 0);
}