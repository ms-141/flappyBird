#ifndef MODEL_H
#define MODEL_H
#include "bird.h"
#include "set_of_pipes.h"
#include "score.h"

typedef struct
{
    Bird bird;
    SetOfPipes pipes[3]; /* 3 sets of pipes will be on the screen at any time */
    Score score;
} Model;

void modelInit(Model *model);

#endif