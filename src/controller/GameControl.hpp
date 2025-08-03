#pragma once

#include "../model/Ball.hpp"
#include "../model/Board.hpp"
#include "../model/Brick.hpp"
#include "../model/GameStats.hpp"
#include "../model/Balls.hpp"
#include "../model/Lasers.hpp"
#include "../model/Racket.hpp"
#include "../utils/GameStates.hpp"
#include "../utils/ScoreLoader.hpp"
#include "../utils/Configs.hpp"
#include <allegro5/allegro.h>
#include <math.h>
#include <iostream>

class GameControl
{
public:
	GameControl() = default;
	void processInputs(Racket &racket, Balls &balls, Board &board, GameStats &stats, Lasers &lasers);
	void update(Board &board, Racket &racket, GameStats &stats, Balls &balls, Lasers &lasers);
	bool isRunning() const;

private:
	ALLEGRO_KEYBOARD_STATE ks_;
	bool mouse_control_ = true;
	bool space_pressed_ = false;
	bool debug_key_pressed_ = false;
	bool running_flag_ = true; // overall game is running by default
	bool game_has_started_ = false;
	bool laser_on_ = false;
	bool release_ball_ = false;
	bool ball_bounce_ = true;
	bool power_interruption_ = false;

	void handleBallLost(GameStats &stats, Racket &racket, Board &board, Balls &balls, Ball &ball, Lasers &lasers);

	void processGameInput(GameStats &stats, Racket &racket, Balls &balls, Lasers &lasers, Board &board);
	void processWelcomeInput();
	void checkWallCollisions(Ball &ball);
	void checkBrickCollisions(Balls &balls, Board &board, GameStats &stats, Racket &racket, Lasers &lasers);
	void checkRacketCollisions(Ball &ball, Racket &racket);
	bool powerUpHitRacket(PowerUps &power_up, Racket &racket);
	bool hasWon(Board &board);
	void resetGame(GameStats &stats, Balls &balls, Racket &racket, Board &board, Lasers &lasers);
	void processEndGameInput();
	void saveBestScore(GameStats &stats, Board &board);
	void handlePowerUps(Brick &brick, Racket &racket, Ball &ball, GameStats &stats, Balls &balls, Lasers &lasers);
	void checkChangeInputControl(Racket &racket);
	void checkDebugInputs(Board &board, GameStats &stats, Lasers &lasers, Balls &balls, Racket &racket);
	void checkLaserCollisions(Brick &brick, Lasers &lasers, GameStats &stats, int c, int r);
	void checkBallCollisions(Balls &balls, Racket &racket, Brick &brick, Lasers &lasers, GameStats &stats, bool vertical_collision, bool horizontal_collision, int c, int r);
};
