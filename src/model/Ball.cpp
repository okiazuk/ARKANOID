#include "model/Ball.hpp"

#include <iostream>

void Ball::update()
{
  positions_.x += direction_.x;
  positions_.y += direction_.y;
}

void Ball::setPosition(float x, float y)
{
  positions_.x = x;
  positions_.y = y;
}

void Ball::setDirection(float dx, float dy)
{
  direction_.x = dx;
  direction_.y = dy;
}

const BallPositions &Ball::getPositions() const { return positions_; }
const BallDirection &Ball::getDirection() const { return direction_; }
const BallParamaters &Ball::getParameters() const { return parameters_; }

void Ball::setRadius(float radius)
{
  parameters_.radius = radius;
}

void Ball::setBallSpeed(float speed)
{
  parameters_.speed = speed;
}

bool Ball::isLost() const
{
  if (positions_.y - parameters_.radius > SCREEN_HEIGHT)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void Ball::reset()
{

  direction_ = BallDirection{};
  parameters_ = BallParamaters{};
}