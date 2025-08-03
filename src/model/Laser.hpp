#pragma once
#include "../utils/Configs.hpp"

#include <vector>

struct LaserPositions
{
	float x1;
	float y1 = RACKET_BOTTOM_POSITION - LASER_DEFAULT_HEIGHT;
	float x2;
	float y2 = RACKET_BOTTOM_POSITION;
};
struct LaserDirection

{

	float x = BALL_DEFAULT_DX; // same as ball default, going vertically
	float y = BALL_DEFAULT_DY;
};

struct LaserParameters
{

	float speed = LASER_DEFAULT_SPEED;
	float width = LASER_DEFAULT_WIDTH;
	float height = LASER_DEFAULT_HEIGHT;
};

class Laser
{

public:
	Laser() = default;
	~Laser();

	void update();
	bool isLost();
	void setPositions(float x1, float x2 = 0, float y1 = 0, float y2 = 0);
	const LaserPositions &getPositions() const;
	const LaserDirection &getDirection() const;
	const LaserParameters &getParameters() const;

private:
	LaserDirection direction_;
	LaserPositions positions_;
	LaserParameters parameters_;
};