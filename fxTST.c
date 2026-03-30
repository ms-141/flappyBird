#include "effects.h"
#include "psg.h"
#include <osbind.h>

int main()
{
    long i;
    long old_ssp = Super(0);

    play_jump_effect();

    for (i = 0; i < 500000; i++);

    play_game_over_effect();

    for (i = 0; i < 500000; i++);

    stop_sound();

    Super(old_ssp);

    return 0;
}