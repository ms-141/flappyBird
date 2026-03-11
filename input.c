/*
 * File: input.c
 *
 * Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
 * Date Created: March 2, 2026
 *
 * Description:
 * This file wraps underlying, O/S-specific code for performing keyboard input.
 *
 * File Status:
 *
*/

#include "input.h"

/* bool processInput(Model *model) {
    if ((model->state == PLAYING) && (model->bird.jump_ticks_remaining == 3)) {
        return true;
    }
    if (model->state == MENU) {
        return true; /* Placeholder for no input */
  /*  }
    if (model->state == GAME_OVER) {
        return true; /* Placeholder for no input */
    /*}
    return false;
}

char nextInput(Model *model) {
    if ((model->state == PLAYING) && (model->bird.jump_ticks_remaining == 3)) {
        return ' '; /* Return the jump key */
   /*}
    if ((model->state == GAME_OVER) || (model->state == PLAYING)) {
        return 'q'; /* End game */
    /*}
} */

int processInput() {
    if (Cconis()) {
        return 1;
    }
    return 0;
}

char nextInput() {
    return (char)Cnecin();
}