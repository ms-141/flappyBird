#include <stdio.h>
#include "model.h"
#include "asynch.h"
#include "synch.h"
#include "cond.h"

/*
 * Test driver for Checkpoint 2.
 * These tests simulate events by calling event handler functions directly
 * (instead of waiting for real keyboard input).
 */

void printModelState(const char *label, const Model *model);
int testModelInit(void);
int testAsynchJump(void);
int testAsynchReset1p(void);
int testSynchBirdMovement(void);
int testSynchPipeMovement(void);
int testSynchPipeRespawn(void);
int testCondTopCollision(void);
int testCondGroundCollision(void);
int testCondPipeCollision(void);
int testCondScoreIncrease(void);

int main()
{
    int passed = 0;
    int total = 0;

    printf("=== Running test driver ===\n");

    total++;
    passed += testModelInit();
    total++;
    passed += testAsynchJump();
    total++;
    passed += testAsynchReset1p();
    total++;
    passed += testSynchBirdMovement();
    total++;
    passed += testSynchPipeMovement();
    total++;
    passed += testSynchPipeRespawn();
    total++;
    passed += testCondTopCollision();
    total++;
    passed += testCondGroundCollision();
    total++;
    passed += testCondPipeCollision();
    total++;
    passed += testCondScoreIncrease();

    printf("=== Summary: %d/%d tests passed ===\n", passed, total);

    return (passed == total) ? 0 : 1;
}

/* Prints all key model fields before/after each test action. */
void printModelState(const char *label, const Model *model)
{
    int i;

    printf("%s\n", label);
    printf("Bird: x=%u y=%d jump_ticks=%u\n", model->bird.x, model->bird.y, model->bird.jump_ticks_remaining);
    for (i = 0; i < 3; i++)
    {
        printf("Pipe[%d]: x=%d y=%u\n", i, model->pipes[i].x, model->pipes[i].y);
    }
    printf("Score: curr=%u high=%u\n", model->score.curr_score, model->score.high_score);
    printf("State: %d\n", model->state);
}

/* Verifies modelInit sets all model fields to expected starting values. */
int testModelInit(void)
{
    int passed = 1;

    Model myModel;
    modelInit(&myModel);

    printModelState("[testModelInit] after modelInit", &myModel);

    if (myModel.bird.x != BIRD_X_POS)
        passed = 0;
    if (myModel.bird.y != BIRD_VERT_START_POS)
        passed = 0;
    if (myModel.bird.jump_ticks_remaining != 0)
        passed = 0;

    if (myModel.pipes[0].x != 0 || myModel.pipes[0].y != 200)
        passed = 0;
    if (myModel.pipes[1].x != 150 || myModel.pipes[1].y != 200)
        passed = 0;
    if (myModel.pipes[2].x != 300 || myModel.pipes[2].y != 200)
        passed = 0;

    if (myModel.score.curr_score != 0)
        passed = 0;
    if (myModel.score.high_score != 0)
        passed = 0;
    if (myModel.state != PLAYING)
        passed = 0;

    printf("TEST modelInit: %s\n", passed ? "PASS" : "FAIL");
    return passed;
}

/* Simulates spacebar jump event by calling handleJump directly. */
int testAsynchJump(void)
{
    int passed = 1;
    Model model;

    modelInit(&model);
    model.bird.jump_ticks_remaining = 0;

    printModelState("[testAsynchJump] before handleJump", &model);
    handleJump(&model);
    printModelState("[testAsynchJump] after handleJump", &model);

    if (model.bird.jump_ticks_remaining != 3)
        passed = 0;

    printf("TEST handleJump: %s\n", passed ? "PASS" : "FAIL");
    return passed;
}

/* Simulates menu '1' input by calling handle1p and checking reset state. */
int testAsynchReset1p(void)
{
    int passed = 1;
    Model model;

    modelInit(&model);
    model.bird.y = 77;
    model.pipes[0].x = 12;
    model.score.curr_score = 9;
    model.score.high_score = 12;
    model.state = MENU;

    printModelState("[testAsynchReset1p] before handle1p", &model);
    handle1p(&model);
    printModelState("[testAsynchReset1p] after handle1p", &model);

    if (model.bird.x != BIRD_X_POS || model.bird.y != BIRD_VERT_START_POS)
        passed = 0;
    if (model.pipes[0].x != 0 || model.pipes[1].x != 150 || model.pipes[2].x != 300)
        passed = 0;
    if (model.score.curr_score != 0 || model.score.high_score != 0)
        passed = 0;
    if (model.state != PLAYING)
        passed = 0;

    printf("TEST handle1p: %s\n", passed ? "PASS" : "FAIL");
    return passed;
}

/* Simulates 4 clock ticks to verify jump-then-fall bird movement logic. */
int testSynchBirdMovement(void)
{
    int passed = 1;
    int tick;
    Model model;

    modelInit(&model);
    handleJump(&model);

    printModelState("[testSynchBirdMovement] before ticks", &model);
    for (tick = 0; tick < 4; tick++)
    {
        handleBirdMovement(&model);
        printf("Tick %d\n", tick + 1);
        printModelState("state", &model);
    }

    if (model.bird.y != (BIRD_VERT_START_POS + (3 * BIRD_JUMP_DELTA) + BIRD_FALL_DELTA))
        passed = 0;
    if (model.bird.jump_ticks_remaining != 0)
        passed = 0;

    printf("TEST handleBirdMovement: %s\n", passed ? "PASS" : "FAIL");
    return passed;
}

/* Simulates one clock tick for pipe movement and checks x-position updates. */
int testSynchPipeMovement(void)
{
    int passed = 1;
    Model model;

    modelInit(&model);
    printModelState("[testSynchPipeMovement] before", &model);
    handlePipeMovement(&model);
    printModelState("[testSynchPipeMovement] after", &model);

    if (model.pipes[0].x != -3)
        passed = 0;
    if (model.pipes[1].x != 147)
        passed = 0;
    if (model.pipes[2].x != 297)
        passed = 0;

    printf("TEST handlePipeMovement: %s\n", passed ? "PASS" : "FAIL");
    return passed;
}

/* Places a pipe off-screen, then verifies respawn behavior after handler call. */
int testSynchPipeRespawn(void)
{
    int passed = 1;
    Model model;

    modelInit(&model);
    model.pipes[0].x = -PIPE_WIDTH;
    model.pipes[0].y = 123;

    printModelState("[testSynchPipeRespawn] before", &model);
    handlePipeRespawn(&model);
    printModelState("[testSynchPipeRespawn] after", &model);

    if (model.pipes[0].x != 640 || model.pipes[0].y != 200)
        passed = 0;

    printf("TEST handlePipeRespawn: %s\n", passed ? "PASS" : "FAIL");
    return passed;
}

/* Forces top boundary collision and checks if game state becomes GAME_OVER. */
int testCondTopCollision(void)
{
    int passed = 1;
    Model model;

    modelInit(&model);
    model.state = PLAYING;
    model.bird.y = 0;

    printModelState("[testCondTopCollision] before", &model);
    handleBirdCollision(&model);
    printModelState("[testCondTopCollision] after", &model);

    if (model.state != GAME_OVER)
        passed = 0;

    printf("TEST handleBirdCollision(top): %s\n", passed ? "PASS" : "FAIL");
    return passed;
}

/* Forces ground collision and checks if game state becomes GAME_OVER. */
int testCondGroundCollision(void)
{
    int passed = 1;
    Model model;

    modelInit(&model);
    model.state = PLAYING;
    model.bird.y = 150 - BIRD_HEIGHT;

    printModelState("[testCondGroundCollision] before", &model);
    handleBirdCollision(&model);
    printModelState("[testCondGroundCollision] after", &model);

    if (model.state != GAME_OVER)
        passed = 0;

    printf("TEST handleBirdCollision(ground): %s\n", passed ? "PASS" : "FAIL");
    return passed;
}

/* Forces pipe collision and checks if game state becomes GAME_OVER. */
int testCondPipeCollision(void)
{
    int passed = 1;
    Model model;

    modelInit(&model);
    model.state = PLAYING;
    model.pipes[0].x = model.bird.x;
    model.pipes[0].y = 200;
    model.bird.y = 100;

    printModelState("[testCondPipeCollision] before", &model);
    handleBirdCollision(&model);
    printModelState("[testCondPipeCollision] after", &model);

    if (model.state != GAME_OVER)
        passed = 0;

    printf("TEST handleBirdCollision(pipe): %s\n", passed ? "PASS" : "FAIL");
    return passed;
}

/* Places one pipe as passed, then verifies score and high score increase. */
int testCondScoreIncrease(void)
{
    int passed = 1;
    Model model;

    modelInit(&model);
    model.score.curr_score = 0;
    model.score.high_score = 0;

    model.pipes[0].x = (int)model.bird.x - PIPE_WIDTH;
    model.pipes[1].x = (int)model.bird.x + 100;
    model.pipes[2].x = (int)model.bird.x + 200;

    printModelState("[testCondScoreIncrease] before", &model);
    handleScoreIncrease(&model);
    printModelState("[testCondScoreIncrease] after", &model);

    if (model.score.curr_score != 1)
        passed = 0;
    if (model.score.high_score != 1)
        passed = 0;

    printf("TEST handleScoreIncrease: %s\n", passed ? "PASS" : "FAIL");
    return passed;
}