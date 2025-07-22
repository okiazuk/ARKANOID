#include "model/Ball.hpp"

#include <iostream>

void Ball::update() {
  positions_.x += direction_.x;
  positions_.y += direction_.y;
}

void Ball::setPosition(int x, int y) {
  positions_.x = x;
  positions_.y = y;
}

void Ball::setDirection(int dx, int dy) {
  direction_.x = dx;
  direction_.y = dy;
}

const BallPositions& Ball::getPositions() const { return positions_; }
const BallDirection& Ball::getDirection() const { return direction_; }
const BallParamaters& Ball::getParameters() const {return parameters_;}


void Ball::setRadius(int radius){
  parameters_.radius = radius;
}

void Ball::setBallSpeed(int speed){
  parameters_.speed = speed;
}

bool Ball::isLost() const{
  if (positions_.y >= SCREEN_HEIGHT){
    return true;
  }else {return false;}
}


void Ball::reset(){

  positions_ = BallPositions{};
  direction_ = BallDirection{};
  parameters_ = BallParamaters{};
}