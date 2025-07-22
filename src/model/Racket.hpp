#pragma once
#include "../utils/Configs.hpp"


struct RacketPositions {
    int x = SCREEN_MIDDLE_POSITION::WIDTH - (RACKET_WIDTH / 2);
    const int y = RACKET_BOTTOM_POSITION;
};

struct RacketParameters {
    int width = RACKET_WIDTH;
    int height = RACKET_HEIGHT;
    int speed = RACKET_SPEED;
};


class Racket {
public:
    Racket() = default;
    void moveLeft();
    void moveRight();
    const RacketPositions& getPositions() const;
    const RacketParameters& getParameters() const;

private:
    RacketPositions positions_{};
    RacketParameters parameters_{};
};
