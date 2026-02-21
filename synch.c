#include "synch.h"
#include "model.h"

void handleBirdMovement(Model *model) {
    Bird *bird = &model->bird;

    if (bird->jump_ticks_remaining > 0) {
        birdJump(bird); 
        bird->jump_ticks_remaining -= 1; /* decrease the number of jump ticks remaining */
    }
    else {
        birdFalling(bird); /* if the bird is not currently jumping, make it fall */
    }
}

void handlePipeMovement(Model *model) {
    unsigned int i;

    for (i = 0; i < 3; i++) {
        movePipes(&model->pipes[i], 3); /* move the pipes to the left by 3 pixels */
    }
}

void handlePipeRespawn(Model *model) {
    unsigned int i;

    for (i = 0; i < 3; i++) {
        if (isOffScreen(&model->pipes[i], 400)) { 
            resetPipes(&model->pipes[i], 400, 200); /* respawn the pipes at the right
                                                    edge of the screen, height isn't random currently */
            increaseScore(&model->score); /* increase the score when the pipes are successfully passed */
        }
    }
}