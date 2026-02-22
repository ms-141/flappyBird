#include "cond.h"
#include "bird.h"
#include "set_of_pipes.h"
#include "score.h"

void handle_bird_collision(Model *model) {
    unsigned int i;

    for (i = 0; i < 3; i++) {
        if (checkBirdCollision(&model->bird, &model->pipes[i], 150)) { /* check for collision with the pipes, ground height is currently 150 */
            model->GameState = GAME_OVER; /* if the bird collides with a pipe, the game is over */
        }
    }
}

void handle_score_increase(Model *model) {
    unsigned int i;

    for (i = 0; i < 3; i++) {
        if (model->pipes[i].x + PIPE_WIDTH <= model->bird.x) { /* if the bird has just passed through the pipes */
            increaseScore(&model->score);
        }
    }
}