#include "Balls.hpp"
#include <algorithm>
#include <memory>





void Balls::createBall(float x, float y, float dx, float dy)
{
    auto new_ball = std::make_unique<Ball>();  // Creates a unique_ptr
    new_ball->setPosition(x, y);
    new_ball->setDirection(dx, dy);
    balls_.push_back(std::move(new_ball));  // Move ownership to the vector
}


std::vector<std::unique_ptr<Ball>>& Balls::getBalls()
{
	return balls_;

}


const std::vector<std::unique_ptr<Ball>>& Balls::getBalls() const {
	return balls_;

}



void Balls::reset(){
    balls_.clear();
}


void Balls::removeBall(const Ball& ball)
{
    auto it = std::remove_if(balls_.begin(), balls_.end(),
        [&ball](const std::unique_ptr<Ball>& b) {
            return b.get() == &ball;  
        });
    balls_.erase(it, balls_.end());
}