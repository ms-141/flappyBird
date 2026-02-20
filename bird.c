#include "bird.h"

void birdJump(Bird *bird) {
    bird->y += BIRD_JUMP_DELTA; /* change vertical pos. by the jump delta */
}

void birdFalling(Bird *bird) {
    bird->y += BIRD_FALL_DELTA; /* change vertical pos. by the fall delta */
}

void update_position(Bird *bird, int jump_pressed) {
    if (jump_pressed) 
        birdJump(bird); /* if the jump button is pressed, make the bird jump */
    else 
        birdFalling(bird); /* otherwise, make the bird fall */
}

int checkCollision(Bird *bird, SetOfPipes *pipes, unsigned int ground_height) {
    if (bird->y >= ground_height) 
        return 1; /* collision with the ground */

    if (bird->y <= 0) 
        return 1; /* collision with the top of the screen */
    
    if (bird->x + BIRD_WIDTH >= pipes->x && bird->x <= pipes->x + PIPE_WIDTH) {
        if (bird->y <= pipes->y || bird->y + BIRD_HEIGHT >= pipes->y + PIPE_GAP_SIZE) {
            return 1; /* Collision with the pipes */
        }
    }

    return 0; /* No collision */
}