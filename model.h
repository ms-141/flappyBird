/*
 * File: model.h
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: February 18, 2026
 *
 * Description:
 * This file defines the data structures and functions for the Model object.
 * This file also contains a struct for the GameState, which is used for determining
 * which screen to display and which conditions to check for.
 * The Model contains the Bird, the Pipes, and the Score.
 *
 * File Status: 
 */

#ifndef MODEL_H
#define MODEL_H
#include "bird.h"
#include "pipepair.h"
#include "score.h"

/* not used here, but used in renderer.c, cond.c and main game loop*/
#define GROUND_HEIGHT 340 

typedef enum
{
    MENU,
    PLAYING,
    GAME_OVER
} GameState;

typedef struct
{
    Bird bird;
    SetOfPipes pipes[3]; /* at most 3 sets of pipes will be on the screen */
    Score score;
    GameState state;
} Model;

void modelInit(Model *model);
void modelReset(Model *model);

#endif