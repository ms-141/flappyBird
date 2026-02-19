#ifndef SET_OF_PIPES_H
#define SET_OF_PIPES_H

typedef struct
{
    unsigned int x; // each set of pipes has a continuously changing horizontal pos.
    unsigned int y; // the vertical pos. of the bottom of the top pipe
    unsigned int gap_size; // the size of the gap between the top and bottom pipe
                           // used to determine the vertical pos. of the top of the bottom pipe                 
} SetOfPipes;

#endif