#pragma once

class GameStats {
public:
    GameStats();

    int getScore() const;
    int getLives() const;

    void addScore(int points);
    void loseLife();
    void reset();

private:
    int score_;
    int lives_;
};