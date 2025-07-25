, 

#include "Board.hpp"

Board::Board(std::vector<std::vector<Brick>>& game_level, int level_number)
    : game_level_(game_level), old_game_level_(game_level), level_number_(level_number)
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

std::vector<std::vector<Brick>>& Board::getBricks() const
{
    return game_level_;
}

void Board::reset(){
    game_level_ = old_game_level_;

}


void Board::changeLevel(const std::vector<std::vector<Brick>>& game_level) 
{
    game_level_ = game_level;
    old_game_level_ = game_level;
    
}

const int Board::getLevelNumber() const{

    return level_number_;

}