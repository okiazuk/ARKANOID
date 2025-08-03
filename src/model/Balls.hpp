#pragma once
#include "Ball.hpp"
#include <memory>
#include <vector>

class Balls
{

public:
	Balls() = default;

	// overloading
	std::vector<std::unique_ptr<Ball>> &getBalls();							// for model
	const std::vector<std::unique_ptr<Ball>> &getBalls() const; // for view
	void createBall(float x = 0, float y = 0, float dx = 0, float dy = 0);
	void removeBall(const Ball &ball);
	void reset();

private:
	std::vector<std::unique_ptr<Ball>> balls_;
};