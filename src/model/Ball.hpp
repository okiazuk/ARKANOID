#pragma once
#include "../utils/Configs.hpp"

struct BallPositions {
    int x = SCREEN_MIDDLE_POSITION::WIDTH;
    int y = BALL_BOTTOM_POSITION;
};

struct BallDirection {
    int x = BALL_DEFAULT_DX;
    int y = BALL_DEFAULT_DY;
};

class Ball {
public:
    Ball();
    void update();
    void setPosition(int x, int y);
    void setDirection(int dx, int dy);
    const BallPositions& getPositions() const;
    const BallDirection& getDirection() const;
    int getRadius() const;

private:
    BallPositions positions_;
    BallDirection direction_;
    int radius_ = BALL_RADIUS;
};