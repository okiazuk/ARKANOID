
#include "PowerUps.hpp"
#include <iostream>




PowerUps::PowerUps(int power_type) 

{

	power_type_ = static_cast<PowerType>(power_type);
}



void PowerUps::update() {

	positions_.x += direction_.x;
	positions_.y += direction_.y;

}



void PowerUps::setPositions(float x, float y){

	positions_.x = x;
	positions_.y = y;
}

const PowerType& PowerUps::getType() const {

	return power_type_;
}

const PowerUpPositions& PowerUps::getPositions() const{

	return positions_;


}

const PowerUpParamaters& PowerUps::getParameters() const{
	return parameters_;
}


void PowerUps::disappear() {

power_type_ = PowerType::NONE;

}


