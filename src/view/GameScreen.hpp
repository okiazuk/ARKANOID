#pragma once
#include "iostream"
#include "model/Ball.hpp"
#include "model/Board.hpp"
#include "model/Brick.hpp"
#include "model/GameStats.hpp"
#include "model/Racket.hpp"
#include "model/Balls.hpp"
#include "model/Lasers.hpp"
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include "../utils/ScoreLoader.hpp"
#include "../utils/GameStates.hpp"

class GameScreen
{
public:
    GameScreen() = default;
    void draw(Board &board, Balls &balls, Racket &racket, GameStats &stats, Lasers& lasers);
    bool init();
    void destroy();

private:
    void drawUI(const GameStats &stats, const Board &board);
    void drawInGame(const Board &board, const Balls &balls, const Racket &racket, const GameStats &stats, const Lasers& lasers);
    void drawEndGame(const GameStats &stats);
    void drawWelcome();
    ALLEGRO_DISPLAY *display_;
    ALLEGRO_FONT *font_;
};