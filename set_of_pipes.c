#include "set_of_pipes.h"

void movePipes(SetOfPipes *pipes, int delta_x) {
    pipes->x -= delta_x; /* Move the pipes to the left by delta_x pixels */
}

void resetPipes(SetOfPipes *pipes, unsigned int new_x, unsigned int new_y) {
    pipes->x = new_x; /* Resets the horizontal position of the pipes to new_x */
    pipes->y = new_y; /* Resets the vertical position of the bottom of the top pipe to new_y */
}

int isOffScreen(SetOfPipes *pipes, unsigned int screen_width) {
    return (pipes->x + PIPE_WIDTH) <= 0; /* returns true if the right edge of the pipes is off screen */
}
