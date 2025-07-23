#pragma once
#include "iostream"
#include "model/Ball.hpp"
#include "model/Board.hpp"
#include "model/Brick.hpp"
#include "model/GameStats.hpp"
#include "model/Racket.hpp"
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>



class GameScreen {
public:
    GameScreen();
    void drawGame(const Board& board, const Ball& ball, const Racket& racket, const GameStats& stats);
    void drawUI(const GameStats& stats);
    void drawEndGame();
    bool init();
    void destroy();


private:
    ALLEGRO_DISPLAY* display_;
    ALLEGRO_FONT* font_;
};