#include <stdio.h>
#include "model.h"
#include "asynch.h"
#include "synch.h"
#include "cond.h"

/*
Authors: Muji Shah, Gurshan Chera, Declan McCarthy
Program Details: flappy bird game!
*/

/* Function prototypes */

/*
void printModelState(void);
void printTestHeader(void);
*/

void testModelInit(void);

/*
void testAsynchJump(void);
void testSynchBirdMovement(void);
void testSynchPipeMovement(void);
void testCondCollisionTop(void);
void testCondCollisionGround(void);
void testCondCollisionPipe(void);
void testCondScoreIncrease(void);
*/

int main()
{

    /* Test calling a test function */
    testModelInit();

    return 0;
}

void testModelInit()
{
    int i;
    int passed = 1;
    
    /* Make a model to test */
    Model myModel;
    modelInit(&myModel);

    /* Printing fields of test model */
    printf("Bird Position: x = %u, y = %d, jumpticks = %u \n", myModel.bird.x, myModel.bird.y, myModel.bird.jump_ticks_remaining);

    /* Pipes test */
    for (i = 0; i < 3; i++)
    {
        printf("Pipe[%d] position: x = %d, y = %u \n", i, myModel.pipes[i].x, myModel.pipes[i].y);
    }
    /* testing score */
    printf("Score: curr = %u high = %u \n", myModel.score.curr_score, myModel.score.high_score);

    if (myModel.bird.x != BIRD_X_POS)
        passed = 0;
    if (myModel.bird.y != BIRD_VERT_START_POS)
        passed = 0;

    if (myModel.pipes[0].x != 200 || myModel.pipes[0].y != 200)
        passed = 0;
    if (myModel.pipes[1].x != 350 || myModel.pipes[1].y != 200)
        passed = 0;
    if (myModel.pipes[2].x != 500 || myModel.pipes[2].y != 200)
        passed = 0;

    if (myModel.score.curr_score != 0)
        passed = 0;

    if (passed)
    {
        printf("TEST modelInit: PASS \n");
    }
    else
    {
        printf("TEST modelInit: FAIL \n");
    }
}