#pragma once

//======================GAME CONSTANTS===========================

//----DISPLAY----

const int UI_TEXT_AREA = 10;
// 16:9 ratio
const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;


struct SCREEN_MIDDLE_POSITION
{
  static constexpr int WIDTH = SCREEN_WIDTH / 2;
  static constexpr int HEIGHT = SCREEN_HEIGHT / 2;
};

//---DEFAULT ELEMENTS SIZE---

//--Racket--

const int RACKET_DEFAULT_WIDTH = 130;
const int RACKET_DEFAULT_HEIGHT = 20;
const int RACKET_DEFAULT_SPEED = 6;
const int RACKET_BOTTOM_POSITION = 800;

//--Ball--

const int BALL_BOTTOM_POSITION = 790;
const int BALL_DEFAULT_RADIUS = 10;
const int BALL_DEFAULT_SPEED = 7;



//-----GAME STATS--------

const int DEFAULT_NUMBER_OF_LIVES = 3; // 3 balls; 0,1,2

