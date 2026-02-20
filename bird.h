#ifndef BIRD_H
#define BIRD_H

typedef struct
{
    unsigned int y; /* bird only changes position vertically */

    int delta_y; /* how much position changes after a clock tick */
} Bird;

#endif