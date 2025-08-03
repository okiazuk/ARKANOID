#include "model/Ball.hpp"

#include <iostream>


Ball::~Ball(){
  std::cout <<"[BALL] DELETED" << std::endl;

}

void Ball::update()
{
  positions_.x += direction_.x*parameters_.speed;
  positions_.y += direction_.y*parameters_.speed;
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


bool Ball::isLost()
{

  bool is_out = positions_.y - parameters_.radius > SCREEN_HEIGHT? true : false;
  is_out_ = is_out;
  return is_out_;
}

void Ball::reset(bool direction)
{

  if (direction){ //choose if we reset direction too
    direction_ = BallDirection{};
  }
  parameters_ = BallParamaters{};

  
}
