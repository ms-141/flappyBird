#include "asynch.h"
#include "model.h"

void handleJump(Model *model) {
    model->bird.jump_ticks_remaining = 3;
}

void handle1p(Model *model) {
    modelInit(model);
}

/* currently the same as handle1p */
void handle2p(Model *model) {
    modelInit(model);
}

/* currently the same as handle1p */
void handleRetry(Model *model) {
    modelInit(model);
}

/*
void handleQuitToMenu(Model *model) {
    modelInit(model);
}
*/

