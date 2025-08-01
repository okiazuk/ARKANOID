#include "../utils/Configs.hpp"





enum class PowerType {

	NONE,					// 0
	LASER,				// 1 - L
	RACKET_GROW,	// 2 - R
	CATCH_BALL,		// 3 - C
	SLOW_DOWN,		// 4 - S
	INTERRUPTION,	// 5 - I
	PLAYER				// 6 - P

};


struct PowerUpDirection {
	float x = static_cast<float>(PU_DEFAULT_DX);
	float y =  static_cast<float>(PU_DEFAULT_SPEED); 

};

struct PowerUpPositions{
	float x;
	float y;

};


struct PowerUpParamaters // same parameters as ball
{
    float radius = static_cast<float>(BALL_DEFAULT_RADIUS);
    float speed = static_cast<float>(PU_DEFAULT_SPEED);
};




class PowerUps{


public:

	PowerUps(int power_type);
	const PowerType& getType() const;
	void setPositions(float x, float y);
	const PowerUpPositions& getPositions() const;
	const PowerUpParamaters& getParameters() const;
	void disappear();
	void update();




private:
	PowerType power_type_;
	PowerUpDirection direction_;
	PowerUpPositions positions_;
	PowerUpParamaters parameters_;

};