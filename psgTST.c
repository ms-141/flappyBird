#include "psg.h"
#include <osbind.h>
#include <stdio.h>

int main()
{
    long i;
    long old_ssp = Super(0);

    /* Test 1: Simple tone */
    set_tone(0, 500);
    set_volume(0, 10);
    enable_channel(0, 1, 0);

    for (i = 0; i < 300000; i++) 
        ;

    /* Test 2: Change pitch */
    set_tone(0, 1000);
    set_volume(0, 10);
    enable_channel(0, 1, 0);

    for (i = 0; i < 300000; i++) 
        ;

    /* Test 3: Noise */
    set_noise(10);
    set_volume(0, 10);
    enable_channel(0, 0, 1);  /* noise only */

    for (i = 0; i < 300000; i++) 
        ;

    /* Test 4: Stop sound */
    stop_sound();

    Super(old_ssp);

    return 0;
}