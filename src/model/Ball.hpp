#pragma once
#include "../utils/Configs.hpp"

struct BallPositions
{
    int x = SCREEN_MIDDLE_POSITION::WIDTH;
    int y = BALL_BOTTOM_POSITION;
};

struct BallDirection
{
    int x = 0;
    int y = -BALL_DEFAULT_SPEED;
};

struct BallParamaters
{
    int radius = BALL_DEFAULT_RADIUS;
    int speed = BALL_DEFAULT_SPEED;
};

class Ball
{
public:
    Ball() = default;
    void update();
    bool isLost() const;
    void reset();
    void setPosition(int x, int y = BALL_BOTTOM_POSITION);
    void setDirection(int dx, int dy);
    void setRadius(int radius);
    void setBallSpeed(int speed);
    const BallPositions &getPositions() const;
    const BallDirection &getDirection() const;
    const BallParamaters &getParameters() const;

private:
    BallPositions positions_;
    BallDirection direction_;
    BallParamaters parameters_;
};