#pragma once
#include "../utils/Configs.hpp"
#include <math.h>

struct BallPositions
{
    float x = static_cast<float>(SCREEN_WIDTH / 2);
    float y = static_cast<float>(BALL_BOTTOM_POSITION);
};

struct BallDirection
{
    float x = static_cast<float>(BALL_DEFAULT_DX);
    float y = static_cast<float>(BALL_DEFAULT_DY);
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
    ~Ball();
    void update();
    bool isLost();
    void reset(bool direction = true);
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
    bool is_out_ = false;
};