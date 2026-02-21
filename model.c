#include "model.h"

void modelInit(Model *model) {
    unsigned int i;
    unsigned int variable_distance = 0; /* the distance between each set of pipes */

    spawnBird(&model->bird);
    for (i = 0; i < 3; i++) {
        resetPipes(&model->pipes[i], 200 + variable_distance, 200); /* the height is currently not random */
        variable_distance += 150; /* increase the distance for the next set of pipes */
    }
    model->score.curr_score = 0;
}