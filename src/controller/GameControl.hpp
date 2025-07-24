#pragma once

#include "../model/Ball.hpp"
#include "../model/Board.hpp"
#include "../model/Brick.hpp"
#include "../model/GameStats.hpp"
#include "../model/Racket.hpp"
#include "../utils/GameStates.hpp"
#include "../utils/ScoreLoader.hpp"
#include "../utils/Configs.hpp"
#include <allegro5/allegro.h>
#include <math.h>
#include <iostream>



class GameControl {
public:
		void processInputs(Racket &racket, Ball &ball);
    void update(Board& board, Ball& ball, Racket& racket, GameStats& stats);
		bool isRunning() const;
		GameStates getGameState() const;
		bool isBallLaunched() const;

private:
	ALLEGRO_KEYBOARD_STATE ks_;
	bool running_flag_ = true; // overall game is running by default
	bool ball_launched_ = false; // whether the ball is launched or not
	void handleBallLost(GameStats& stats, Ball& ball, Racket& racket, Board& board);

	void processGameInput(Racket& racket, Ball& ball);
	void processMenuInput();
	void checkWallCollisions(Ball& ball);
	void checkBrickCollisions(Ball& ball,  Board& board, GameStats& stats);
	void checkRacketCollisions(Ball& ball, Racket& racket);
	bool hasWon(Board& board);
	void resetGame(GameStats &stats, Ball &ball, Racket &racket, Board &board);
	void saveBestScore(GameStats& stats);

};
