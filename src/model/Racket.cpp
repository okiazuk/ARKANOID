#include "model/Racket.hpp"


void Racket::moveLeft() { 
    
    positions_.x -= parameters_.speed; }

void Racket::moveRight() {
    positions_.x += parameters_.speed;
}

const RacketPositions& Racket::getPositions() const { return positions_; }

const RacketParameters& Racket::getParameters() const { return parameters_; }
