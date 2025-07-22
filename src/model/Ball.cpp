#include "model/Ball.hpp"

// Initialize ball at origin moving up-right with default radius
Ball::Ball()
{
}

void Ball::update()
{
    positions_.x += direction_.x;
    positions_.y += direction_.y;
}

void Ball::setPosition(int x, int y)
{
    positions_.x = x;
    positions_.y = y;
}

void Ball::setDirection(int dx, int dy)
{
    direction_.x = dx;
    direction_.y = dy;
}

const BallPositions& Ball::getPositions() const {return positions_;}
const BallDirection& Ball::getDirection() const {return direction_;}


int Ball::getRadius() const { return radius_; }
