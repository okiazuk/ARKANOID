#pragma once
#include <vector>
#include "Brick.hpp"


struct BoardParameters{
    int width;
    int height;
};



class Board {
public:
    Board(std::vector<std::vector<Brick>>& level);
    void changeLevel(const std::vector<std::vector<Brick>>& level);
    void reset();
    std::vector<std::vector<Brick>>& getBricks() const;
    const BoardParameters& getParameters() const;


private:
    BoardParameters parameters_;
    std::vector<std::vector<Brick>>& game_level_;
    std::vector<std::vector<Brick>> old_game_level_; // non modified level


};