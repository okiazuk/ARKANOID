#pragma once
#include <vector>
#include "Brick.hpp"


struct BoardParameters{
    int width;
    int height;
};



class Board {
public:
    Board(const std::vector<std::vector<Brick>>& level);
    void changeLevel(const std::vector<std::vector<Brick>>& level);
    const std::vector<std::vector<Brick>>& getBricks() const;
    const BoardParameters& getParameters() const;


private:
    BoardParameters parameters_;
    std::vector<std::vector<Brick>> game_level_;
};