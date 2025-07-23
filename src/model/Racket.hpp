#pragma once
#include "../utils/Configs.hpp"

struct RacketPositions
{
    float x = SCREEN_WIDTH/2 - (RACKET_DEFAULT_WIDTH / 2);
    float y = RACKET_BOTTOM_POSITION;
};

struct RacketParameters
{
    int width = RACKET_DEFAULT_WIDTH;
    int height = RACKET_DEFAULT_HEIGHT;
    float speed = RACKET_DEFAULT_SPEED;
};

class Racket
{
public:
    Racket() = default;
    void moveLeft();
    void moveRight();
    void reset();
    void setPosition(float x);
    const RacketPositions &getPositions() const;
    const RacketParameters &getParameters() const;
    void setParameters(int width, int height, float speed); // if in the future we want to customize the racket

private:
    RacketPositions positions_;
    RacketParameters parameters_;
};
