

#include "Board.hpp"

Board::Board(const std::vector<std::vector<Brick>>& game_level)
    : game_level_(game_level)
{
    if (game_level.empty()) {
        parameters_.width = 0;
        parameters_.height = 0;
    } else {
        parameters_.width = game_level[0].size();
        parameters_.height = game_level.size();
    }
}

const BoardParameters& Board::getParameters() const {
    return parameters_;
}

const std::vector<std::vector<Brick>>& Board::getBricks() const
{
    return game_level_;
}


void Board::changeLevel(const std::vector<std::vector<Brick>>& game_level) 
{
    game_level_ = game_level;
}