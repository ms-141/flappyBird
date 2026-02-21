#include "score.h"

increaseScore(Score *score) {
    score->curr_score += 1;
    
    if (score->curr_score > score->high_score) {
        score->high_score = score->curr_score; /* update high score if current score exceeds it */
    }
}