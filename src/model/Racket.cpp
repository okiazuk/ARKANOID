#include "model/Racket.hpp"

void Racket::moveLeft()
{
    positions_.x -= parameters_.speed;
}

void Racket::moveRight()
{
    positions_.x += parameters_.speed;
}

const RacketPositions &Racket::getPositions() const { return positions_; }

const RacketParameters &Racket::getParameters() const { return parameters_; }

void Racket::setParameters(int width, int height, int speed)
{
    parameters_.width = width;
    parameters_.height = height;
    parameters_.speed = speed;
}

void Racket::reset()
{

    parameters_ = RacketParameters{};
}

void Racket::setPosition(float x)
{
    positions_.x = x;
}