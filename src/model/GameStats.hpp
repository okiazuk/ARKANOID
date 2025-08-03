#pragma once
#include "../utils/Configs.hpp"
#include <string>


struct BasicInfo {
    int score = 0;
    int lives = DEFAULT_NUMBER_OF_LIVES;
    bool game_over = false;
};


class GameStats {
public:
    GameStats() = default;

    const BasicInfo& getBasicInfos() const;
    void addScore(int points);
    void loseLife();
    void gainLife();
    void setGameOverFlag(bool flag);
    void reset();

private:
    BasicInfo basic_infos_;
};