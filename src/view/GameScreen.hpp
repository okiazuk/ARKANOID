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
#include "../utils/ScoreLoader.hpp"
#include "../utils/GameStates.hpp"




class GameScreen {
public:
    GameScreen() = default;
    void draw(Board& board, Ball& ball, Racket& racket, GameStats& stats);
    bool init();
    void destroy();


private:
    void drawUI(const GameStats& stats, const Board& board);
    void drawInGame(const Board& board, const Ball& ball, const Racket& racket, const GameStats& stats);
    void drawEndGame(const GameStats& stats);
    void drawMainMenu();
    ALLEGRO_DISPLAY* display_;
    ALLEGRO_FONT* font_;

};