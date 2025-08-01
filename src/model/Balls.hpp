#pragma once
#include "Ball.hpp"
#include <memory>
#include <vector>




class Balls {

public:

	Balls();
	~Balls();

	std::vector<Ball*>& getBalls();
	const std::vector<Ball*>& getBalls() const;
	void createBall(float x=0, float y=0, float dx=0, float dy=0);
	void removeBall(const Ball& ball);
	void reset();

private:		

	std::vector<Ball*> balls_;



};