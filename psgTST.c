#include "psg.h"
#include <osbind.h>

int main()
{
    long old_ssp = Super(0);

    set_tone(0, 500);        // channel A
    set_volume(0, 10);       // audible volume
    enable_channel(0, 1, 0); // tone ON, noise OFF

    Super(old_ssp);

    return 0;
}