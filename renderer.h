/*
 * File: renderer.h
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: February 28, 2026
 *
 * Description:
 * This file declares the functions for the Renderer routines.
 *
 * File Status: 
*/

#ifndef RENDERER_H
#define RENDERER_H

#include <stdio.h>
#include "model.h"
#include "raster.h"

void render(Model *model, UINT8 *base);

void render_bird(const Bird *bird, UINT8 *base);
void render_pipe(const SetOfPipes *pipes, UINT8 *base);
void render_score(Score *score, UINT8 *base);

#endif