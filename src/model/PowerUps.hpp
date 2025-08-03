#include "../utils/Configs.hpp"

enum class PowerType
{

	NONE,					// 0
	LASER,				// 1 - L - shoot lasers which destroy an entire column of bricks, best power up of them (SPACE BAR)
	RACKET_GROW,	// 2 - R - racket grow, stackable
	CATCH_BALL,		// 3 - C - catch the ball when it hits racket
	SLOW_DOWN,		// 4 - S - slow down the ball, stackable
	INTERRUPTION, // 5 - I - balls splits in 3, no more power ups received
	PLAYER				// 6 - P - player receives extra life, stackable

};

struct PowerUpDirection
{
	float x = static_cast<float>(PU_DEFAULT_DX);
	float y = static_cast<float>(PU_DEFAULT_SPEED);
};

struct PowerUpPositions
{
	float x;
	float y;
};

struct PowerUpParamaters // same parameters as ball
{
	float radius = static_cast<float>(BALL_DEFAULT_RADIUS);
	float speed = static_cast<float>(PU_DEFAULT_SPEED);
};

class PowerUps
{

public:
	PowerUps(int power_type);
	const PowerType &getType() const;
	void setPositions(float x, float y);
	const PowerUpPositions &getPositions() const;
	const PowerUpParamaters &getParameters() const;
	void destroy();
	void update();

private:
	PowerType power_type_;
	PowerUpDirection direction_;
	PowerUpPositions positions_;
	PowerUpParamaters parameters_;
};