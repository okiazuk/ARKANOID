#pragma once
#include <vector>
#include "Brick.hpp"

class Board {
public:
    Board(const std::vector<std::vector<Brick>>& level);
    const std::vector<std::vector<Brick>>& getBricks() const;
    const int getHeight() const;
    const int getWidth() const;
    


private:
    const int width_;
    const int height_;
    const std::vector<std::vector<Brick>>& bricks_;
};