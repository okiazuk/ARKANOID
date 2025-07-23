#pragma once

#include "../model/Ball.hpp"
#include "../model/Board.hpp"
#include "../model/Brick.hpp"
#include "../model/GameStats.hpp"
#include "../model/Racket.hpp"
#include <allegro5/allegro.h>
#include <math.h>
#include <iostream>



class GameControl {
public:
    void processGameInput(Racket& racket, Ball& ball);
		void processMenuInput();
    void update(Board& board, Ball& ball, Racket& racket, GameStats& stats);
		bool isRunning() const;
		bool isGameRunning() const;
		bool isBallLaunched() const;
		void handleBallLost(GameStats& stats, Ball& ball, Racket& racket, Board& board);

private:
	ALLEGRO_KEYBOARD_STATE ks_;
	bool running_flag_ = true; // overall game is running by default
	bool game_running_flag_ = true; // level game is running
	bool ball_launched_ = false; // whether the ball is launched or not 
	void checkWallCollisions(Ball& ball);
	void checkBrickCollisions(Ball& ball,  Board& board, GameStats& stats);
	void checkRacketCollisions(Ball& ball, Racket& racket);



};
