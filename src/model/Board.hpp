#pragma once
#include "Brick.hpp"
#include "../utils/LevelLoader.hpp"
#include <vector>



struct BoardParameters{
    int width;
    int height;
};


inline std::vector<std::vector<Brick>> default_level = loadLevel(LEVEL_PATH_MAP[0]);


class Board {
public:
    Board(std::vector<std::vector<Brick>>& level = default_level, int level_number = 0);
    void changeLevel(const std::vector<std::vector<Brick>>& level, int level_number);
    void reset();
    std::vector<std::vector<Brick>>& getBricks() const;
    const int getLevelNumber() const;
    const BoardParameters& getParameters() const;


private:
    BoardParameters parameters_;
    std::vector<std::vector<Brick>>& game_level_;
    int level_number_;
    std::vector<std::vector<Brick>> old_game_level_; // non modified level


};