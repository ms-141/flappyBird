#ifndef SET_OF_PIPES_H
#define SET_OF_PIPES_H

#define PIPE_WITDH 32
#define PIPE_GAP_SIZE 100

typedef struct
{
    int x; /* each set of pipes has a continuously changing horizontal pos. */
    unsigned int y; /* the vertical pos. of the bottom of the top pipe.
                    the pipe gap size will be added to this value to 
                    determine the vertical pos. of the top of the bottom pipe */
                
} SetOfPipes;

/* moves the pipes by the given speed (delta_x) */
void movePipes(SetOfPipes *pipes, int delta_x); 
/* deletes pipes that are off screen and spawns new pipes with a new height */
void resetPipes(SetOfPipes *pipes, unsigned int new_x, unsigned int new_y); 
/* checks if the pipes are off screen (x < 0) */
int isOffScreen(SetOfPipes *pipes, unsigned int screen_width);

#endif