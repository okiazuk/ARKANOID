#pragma once

#include "../model/Ball.hpp"
#include "../model/Board.hpp"
#include "../model/Brick.hpp"
#include "../model/GameStats.hpp"
#include "../model/Racket.hpp"

class GameControl {
public:
    void processInput(Racket& racket);
    void update(Board& board, Ball& ball, Racket& racket, GameStats& stats);
};