#pragma once
#include "../utils/Configs.hpp"

struct BallPositions
{
    float x = static_cast<float>(SCREEN_WIDTH / 2);
    float y = static_cast<float>(BALL_BOTTOM_POSITION);
};

struct BallDirection
{
    float x = 0;
    float y = static_cast<float>(-BALL_DEFAULT_SPEED);
};

struct BallParamaters
{
    float radius = static_cast<float>(BALL_DEFAULT_RADIUS);
    float speed = static_cast<float>(BALL_DEFAULT_SPEED);
};

class Ball
{
public:
    Ball() = default;
    void update();
    bool isLost() const;
    void reset();
    void setPosition(float x, float y = static_cast<float>(BALL_BOTTOM_POSITION));
    void setDirection(float dx, float dy);
    void setRadius(float radius);
    void setBallSpeed(float speed);
    const BallPositions &getPositions() const;
    const BallDirection &getDirection() const;
    const BallParamaters &getParameters() const;

private:
    BallPositions positions_;
    BallDirection direction_;
    BallParamaters parameters_;
};