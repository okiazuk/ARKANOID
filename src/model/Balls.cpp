#include "Balls.hpp"
#include <algorithm>
#include <memory>


Balls::Balls()
{
}

Balls::~Balls() {
    for (Ball* ball : balls_) {
        delete ball;
    }
}


void Balls::createBall(float x, float y, float dx, float dy)
{
    Ball* new_ball = new Ball();
    new_ball->setPosition(x, y);
    new_ball->setDirection(dx, dy);
    balls_.push_back(new_ball);
}


std::vector<Ball*>& Balls::getBalls()
{
	return balls_;

}


const std::vector<Ball*>& Balls::getBalls() const {
	return balls_;

}



void Balls::reset(){
    for (auto& ball: balls_){
        delete ball;
    }
    balls_.clear();
}


void Balls::removeBall(const Ball& ball){
    auto it = std::remove_if(balls_.begin(), balls_.end(),
                             [&ball](const Ball* b) {
                                 if (b == &ball) {
                                     delete b;
                                     return true;
                                 }
                                 return false;
                             });
    balls_.erase(it, balls_.end());

}