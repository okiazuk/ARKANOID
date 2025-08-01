#include "Laser.hpp"
#include <iostream>




Laser::~Laser(){
	std::cout << "[LASER] DELETED" << std::endl;
}



void Laser::update(){

	positions_.x1 += direction_.x*parameters_.speed;
	positions_.y1 += direction_.y*parameters_.speed;
	positions_.x2 += direction_.x*parameters_.speed;
	positions_.y2 += direction_.y*parameters_.speed;
}

void Laser::setPositions(float x1, float x2, float y1, float y2){
	positions_.x1 = x1;
	positions_.x2 = x1 + LASER_DEFAULT_WIDTH;
}


const LaserDirection& Laser::getDirection() const{
	return direction_;
}

const LaserPositions& Laser::getPositions() const {

	return positions_;
}


const LaserParameters& Laser::getParameters() const {
	return parameters_;
}

bool Laser::isLost(){

	if (positions_.y1 <= SEPARATION_LINE_HEIGHT){
		return true;
	}else {
		return false;
	}
}


