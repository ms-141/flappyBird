#include "model.h"

void modelInit(Model *model) {
    unsigned int i;

    spawnBird(&model->bird);
    for (i = 0; i < 3; i++) {
        resetPipes(&model->pipes[i], 200 + i * 150, 200); /* the height is currently not random */
    }
    model->score.curr_score = 0;
}