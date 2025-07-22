#pragma once

#include "../model/Ball.hpp"
#include "../model/Board.hpp"
#include "../model/Brick.hpp"
#include "../model/GameStats.hpp"
#include "../model/Racket.hpp"



class GameControl {
public:
    void processInput(Racket& racket, Ball& ball);
    void update(Board& board, Ball& ball, Racket& racket, GameStats& stats);
		bool isGameRunning() const;
		bool isGameStarting() const;
		void handleBallLost(GameStats& stats, Ball& ball, Racket& racket);

private:
	bool running_flag_ = true; // game is running by default
	bool game_starting_flag_ = false; // breakout game is not running by default
	void checkWallCollisions(Ball& ball);
	void checkBrickCollisions(Ball& ball,  Board& board, GameStats& stats);
	void checkRacketCollisions(Ball& ball, Racket& racket);



};
