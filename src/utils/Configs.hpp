#pragma once

/**
 * @brief using constexpr for better performance
 */


//======================GAME CONSTANTS===========================

//----DISPLAY----

// 16:9 ratio
constexpr int SCREEN_WIDTH = 1600;
constexpr int SCREEN_HEIGHT = 900;


//-- UI score, lives, lines, etc
constexpr int UI_TEXT_HEIGHT = 10;
constexpr int UI_TEXT_WIDTH_LIVES = (SCREEN_WIDTH - 150) / 2;
constexpr int SEPARATION_LINE_HEIGHT = UI_TEXT_HEIGHT*3;

//---DEFAULT ELEMENTS PARAMETERS---

//--Racket--

constexpr int RACKET_DEFAULT_WIDTH = 130;
constexpr int RACKET_DEFAULT_HEIGHT = 20;
constexpr int RACKET_DEFAULT_SPEED = 6;
constexpr int RACKET_BOTTOM_POSITION = 800;

//--Ball--

constexpr int BALL_BOTTOM_POSITION = 790;
constexpr int BALL_DEFAULT_RADIUS = 10;
constexpr int BALL_DEFAULT_SPEED = 7;


//--Brick--

constexpr int BRICK_DEFAULT_WIDTH = 60;
constexpr int BRICK_DEFAULT_HEIGHT = 20;

//-points gained per brick-

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

constexpr int DEFAULT_NUMBER_OF_LIVES = 3; // 3 balls; 0,1,2


