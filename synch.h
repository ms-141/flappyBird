/*
 * File: synch.h
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: February 21, 2026
 *
 * Description:
 * This file defines the functions for handling synchronous events, such as 
 * the movement of the bird and pipes, and the respawning of the pipes.
 *
 * File Status: 
*/

#ifndef SYNCH_H
#define SYNCH_H
#include "model.h"

void handleBirdMovement(Model *model);
void handlePipeMovement(Model *model);
void handlePipeRespawn(Model *model);

#endif