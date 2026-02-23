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
#include "set_of_pipes.h"
#include "score.h"

typedef enum {
    MENU,
    GAME,
    GAME_OVER
} GameState;

typedef struct
{
    Bird bird;
    SetOfPipes pipes[3]; /* 3 sets of pipes will be on the screen at any time */
    Score score;
} Model;

void modelInit(Model *model);

#endif