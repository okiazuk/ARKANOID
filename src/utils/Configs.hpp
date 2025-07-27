#pragma once
#include <string>
#include <map>
/**
 * @brief using constexpr for better performance
 */


//======================GAME CONSTANTS===========================

//--number of bricks and rows
constexpr int NUMBER_OF_BRICKS_PER_ROW = 14;
constexpr int NUMBER_OF_ROW = 7;


//----DISPLAY----

//--Brick--

constexpr int BRICK_DEFAULT_WIDTH = 80;
constexpr int BRICK_DEFAULT_HEIGHT = 30;


// 16:9 ratio
constexpr int SCREEN_WIDTH = NUMBER_OF_BRICKS_PER_ROW * BRICK_DEFAULT_WIDTH;
constexpr int SCREEN_HEIGHT = 900;


//-- UI score, lives, lines, game over
constexpr int UI_TEXT_INFO_HEIGHT = 10;
constexpr int UI_TEXT_INFO_WIDTH_BETWEEN_WALLS = 20;
constexpr int SEPARATION_LINE_HEIGHT = UI_TEXT_INFO_HEIGHT*6;


//--Racket--

constexpr int RACKET_DEFAULT_WIDTH = 120;
constexpr int RACKET_DEFAULT_HEIGHT = 20;
constexpr int RACKET_DEFAULT_SPEED = 5;
constexpr int RACKET_BOTTOM_POSITION = 800;

//--Ball--

constexpr int BALL_BOTTOM_POSITION = 790;
constexpr int BALL_DEFAULT_RADIUS = 7;
constexpr int BALL_DEFAULT_SPEED = 6;
constexpr int BALL_DEFAULT_DX = 0;

//--Power Ups--

constexpr int PU_DEFAULT_DY = 1;
constexpr int PU_DEFAULT_DX = 0;
constexpr int PU_DEFAULT_SPEED = 4;


//-Points gained per brick-

constexpr int WHITE_POINTS = 50;
constexpr int ORANGE_POINTS = 60;
constexpr int CYAN_POINTS = 70;
constexpr int GREEN_POINTS = 80;
constexpr int RED_POINTS = 90;
constexpr int BLUE_POINTS = 100;
constexpr int MAGENTA_POINTS = 110;
constexpr int YELLOW_POINTS = 120;
constexpr int SILVER_POINTS = 200;


//-----GAME STATS--------

constexpr int DEFAULT_NUMBER_OF_LIVES = 1; // 3 balls; 0,1,2
constexpr int TOTAL_LEVELS = 4;



//---MAPS-----


inline std::map<int, std::string> BEST_SCORE_PATH_MAP =
{
	{0, "../src/model/levels/best_score_1"},
	{1, "../src/model/levels/best_score_2"},
	{2, "../src/model/levels/best_score_3"},
	{3, "../src/model/levels/best_score_4"}
};

inline std::map<int, std::string> LEVEL_PATH_MAP = {
	{0, "../src/model/levels/level_1"},
	{1, "../src/model/levels/level_2"},
	{2, "../src/model/levels/level_3"},
	{3, "../src/model/levels/level_4"}
};

inline std::map<int, char> POWER_UP_TO_LETTER_MAP = {

	{1, 'L'},
	{2, 'R'},
	{3, 'C'},
	{4, 'S'},
	{5, 'I'},
	{6, 'P'}

};