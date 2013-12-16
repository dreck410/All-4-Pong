#include "score.h"

#include <vector>

// increase currentScore by currentLevelIncrease and
// returns the updated currentScore
void Score::increaseScore() {
    currentScore = currentScore + currentLevelIncrease;
}

// decrease currentScore by currentLevelDecrease and
// returns the updated currentScore
void Score::decreaseScore() {
    currentScore = currentScore - currentLevelDecrease;
}
