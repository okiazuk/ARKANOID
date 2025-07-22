

#include "Board.hpp"

Board::Board(const std::vector<std::vector<Brick>>& level)
    : bricks_(level)
    , height_(0)
    , width_(0)
{
}

const int Board::getWidth() const
{
    return width_;
}

const int Board::getHeight() const
{
    return height_;
}

const std::vector<std::vector<Brick>>& Board::getBricks() const
{
    return bricks_;
}