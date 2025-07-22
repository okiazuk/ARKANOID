#pragma once
#include "iostream"
#include "model/Ball.hpp"
#include "model/Board.hpp"
#include "model/Brick.hpp"
#include "model/GameStats.hpp"
#include "model/Racket.hpp"
#include <allegro5/allegro_primitives.h>



class GameScreen {
public:
    GameScreen();
    void draw(const Board& board, const Ball& ball, const Racket& racket, const GameStats& stats);
    bool init();
    void destroy();


private:
    ALLEGRO_DISPLAY* display_;
};