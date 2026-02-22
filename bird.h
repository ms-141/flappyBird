#ifndef BIRD_H
#define BIRD_H

#define BIRD_VERT_START_POS 200
#define BIRD_X_POS 50
#define BIRD_JUMP_DELTA -15 /* bird will move up by this many pixels upon user input for 3 ticks (45 tot) */
#define BIRD_FALL_DELTA 1 /* bird will move down by this many pixels after each clock tick when not jumping */
#define BIRD_WIDTH 16
#define BIRD_HEIGHT 16

typedef struct
{
    unsigned int x; /* bird's horizontal pos. is fixed */
    int y; /* the birds vertical pos. changes */
    unsigned int jump_ticks_remaining; /* the number of ticks remaining in the current jump */
} Bird;

/* initializes the bird's position at the start of the game */
void spawnBird(Bird *bird); 
/* makes the bird jump upwards by subtracting from its y pos. */
void birdJump(Bird *bird); 
/* makes the bird fall downwards by adding to its y pos. */  
void birdFalling(Bird *bird); 
/* checks if the bird has collided with the pipes, the ground, or the top of the screen (y = 0) 
1 = collided, 0 = hasn't collided */
int checkBirdCollision(Bird *bird, SetOfPipes *pipes, unsigned int ground_height);

#endif